// network communication as a task and accepting incoming connections recipes
#include <memory>
#include <stdexcept>
#include <iostream>
#include <boost/array.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/thread/thread.hpp>

namespace detail {

template <typename T>
struct task_wrapped
{
public:
	explicit task_wrapped(T const & task_unwrapped)
		: _task_unwrapped{task_unwrapped}
	{}

	void operator()() const {
		try {
			boost::this_thread::interruption_point();
		}
		catch (boost::thread_interrupted const &) {}

		try {
			_task_unwrapped();  // executing task
		}
		catch (std::exception const & e) {
			std::cerr << "exception: " << e.what() << std::endl;
		}
		catch (boost::thread_interrupted const & e) {
			std::cerr << "Thread interrupted" << std::endl;
		}
		catch (...) {
			std::cerr << "Unknown exception" << std::endl;
		}
	}

private:
	T _task_unwrapped;
};

template <typename T>
task_wrapped<T> make_task_wrapped(T const & task_unwrapped)  // we do not need make_xxx in C++17
{
	return task_wrapped<T>{task_unwrapped};
}

template <typename F>
class timer_task
{
public:
	explicit timer_task(std::unique_ptr<boost::asio::deadline_timer> timer, F const & task_unwrapped)
		: _timer{std::move(timer)}
		, _task{task_unwrapped}
	{}

	void operator()(boost::system::error_code const & error) const
	{
		if (!error)
			_task();
		else
			std::cerr << error << std::endl;
	}

private:
	std::unique_ptr<boost::asio::deadline_timer> _timer;
	task_wrapped<F> _task;
};

}  // detail

struct connection_with_data : boost::noncopyable
{
	explicit connection_with_data(boost::asio::io_context & io)
		: socket{io}
	{}

	template <typename Executor>
	explicit connection_with_data(Executor executor)
		: socket{executor}
	{}

	void shutdown()
	{
		if (!socket.is_open())
			return;

		boost::system::error_code ignore;
		socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignore);
		socket.close(ignore);
	}

	~connection_with_data() {shutdown();}

	boost::asio::ip::tcp::socket socket;
	std::string data;
};

using connection_ptr = std::unique_ptr<connection_with_data>;

template<typename T>
class task_wrapped_with_connection
{
public:
	explicit task_wrapped_with_connection(connection_ptr && c, T const & f)
		: _c{std::move(c)}
		, _task_unwrapped{f}
	{}

	void operator()(boost::system::error_code const & error, size_t bytes_count)
	{
		auto const lambda = [this, &error, bytes_count]{
			this->_c->data.resize(bytes_count);
			this->_task_unwrapped(std::move(this->_c), error);
		};

		auto const task = detail::make_task_wrapped(lambda);
		task();
	}

private:
	connection_ptr _c;
	T _task_unwrapped;
};

template <typename Functor>
void async_write_data(connection_ptr && c, Functor const & f)
{
	boost::asio::ip::tcp::socket & s = c->socket;
	std::string & d = c->data;

	boost::asio::async_write(s, boost::asio::buffer(d),
		task_wrapped_with_connection<Functor>{std::move(c), f});
}

template <typename Functor>
void async_read_data(connection_ptr && c, Functor const & f, size_t at_least_bytes)
{
	c->data.resize(at_least_bytes);

	boost::asio::ip::tcp::socket & s = c->socket;
	std::string & d = c->data;
	char * p = (d.empty() ? nullptr : &d[0]);

	boost::asio::async_read(s, boost::asio::buffer(p, d.size()),
		task_wrapped_with_connection<Functor>{std::move(c), f});
}

template <typename Functor>
void async_read_data_at_least(connection_ptr && c, Functor const & f, size_t at_least_bytes, size_t at_most)
{
	c->data.resize(at_most);

	boost::asio::ip::tcp::socket & s = c->socket;
	std::string & d = c->data;
	char * p = (at_most == 0 ? nullptr : &d[0]);

	boost::asio::async_read(s, boost::asio::buffer(p, at_most),
		boost::asio::transfer_at_least(at_least_bytes),
		task_wrapped_with_connection<Functor>{std::move(c), f});
}

class tasks_processor : private boost::noncopyable
{
public:
	template <typename T>
	static void push_task(T const & task_unwrapped)
	{
		get_io().post(detail::make_task_wrapped(task_unwrapped));
	}

	static void start() {get_io().run();}
	static void stop() {get_io().stop();}

	template <typename Time, typename Func>
	static void run_delayed(Time duration_or_time, Func const & f)
	{
		std::unique_ptr<boost::asio::deadline_timer> timer{
			new boost::asio::deadline_timer{get_io(), duration_or_time}
		};

		boost::asio::deadline_timer & timer_ref = *timer;

		timer_ref.async_wait(
			detail::timer_task<Func>(std::move(timer), f));
	}

	static connection_ptr create_connection(char const * addr, unsigned short port_num)
	{
		connection_ptr c{new connection_with_data{get_io()}};

		c->socket.connect(boost::asio::ip::tcp::endpoint{
			boost::asio::ip::address_v4::from_string(addr), port_num});

		return c;
	}

	template <class Functor>
	static void add_listener(unsigned short port_num, Functor const & f)
	{
		std::unique_ptr<tcp_listener> listener{new tcp_listener{get_io(), port_num, f}};
		start_accepting_connection(std::move(listener));
	}

	static void start_multiple(size_t threads_count = 0)
	{
		if (!threads_count)
			threads_count = std::max(static_cast<int>(boost::thread::hardware_concurrency()), 1);

		--threads_count;  // first thread is the current thread

		boost::asio::io_context & io = get_io();
		boost::thread_group tg;
		for (size_t i = 0; i < threads_count; ++i)
			tg.create_thread([&io]{io.run();});

		io.run();
		tg.join_all();
	}

	/* This function is not thread safe!
	Must be called before all the `start()` calls.
	Function can be called only once. */
	template <typename Func>
	static void register_signals_handler(Func const & f, std::initializer_list<int> signals_to_wait)
	{
		// making shure that this is the first call
		assert(!signal_handler());

		signal_handler() = f;
		boost::asio::signal_set & sigs = signals();

		std::for_each(signals_to_wait.begin(), signals_to_wait.end(),
			[&sigs](int signal) {
				sigs.add(signal);
			});

		sigs.async_wait(&tasks_processor::handle_signals);
	}

protected:
	static boost::asio::io_context & get_io()
	{
		static boost::asio::io_context io;
		static boost::asio::io_context::work idle{io};
		return io;
	}

	static boost::asio::signal_set & signals()
	{
		static boost::asio::signal_set _signals{get_io()};
		return _signals;
	}

	static boost::function<void (int)> & signal_handler()
	{
		static boost::function<void (int)> _users_signal_handler;
		return _users_signal_handler;
	}

private:
	static void handle_signals(boost::system::error_code const & error, int signal_number)
	{
		signals().async_wait(&tasks_processor::handle_signals);

		if (error)
			std::cerr << "Error in signal handling: " << error << '\n';
		else
		{
			boost::function<void (int)> h = signal_handler();

			detail::make_task_wrapped([h, signal_number]{
				h(signal_number);
			})();
		}
	}

	using acceptor_t = boost::asio::ip::tcp::acceptor;

	using on_accept_func_t = boost::function<void(connection_ptr, boost::system::error_code const &)>;

	struct tcp_listener
	{
		acceptor_t _acceptor;
		on_accept_func_t const _func;
		connection_ptr _new_c;

		template <typename Functor>
		tcp_listener(boost::asio::io_context & io_service, unsigned short port, Functor const & task_unwrapped)
			: _acceptor{io_service, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}}
			, _func{task_unwrapped}
		{}
	};

	using listener_ptr = std::unique_ptr<tcp_listener>;

	struct handle_accept
	{
		listener_ptr listener;

		explicit handle_accept(listener_ptr && l)
			: listener{std::move(l)}
		{}

		void operator()(boost::system::error_code const & error)
		{
			task_wrapped_with_connection<on_accept_func_t> task{std::move(listener->_new_c),
				listener->_func};

			start_accepting_connection(std::move(listener));
			task(error, 0);
		}
	};

	static void start_accepting_connection(listener_ptr && listener)
	{
		if (!listener->_acceptor.is_open())
			return;

		listener->_new_c.reset(new connection_with_data{
#if BOOST_VERSION >= 107000
			listener->_acceptor.get_executor()
#else
			listener->_acceptor.get_io_service()
#endif
			});

		boost::asio::ip::tcp::socket & s = listener->_new_c->socket;
		acceptor_t & a = listener->_acceptor;
		a.async_accept(s, tasks_processor::handle_accept{std::move(listener)});
	}

};  // task_processor


bool g_authed = false;
unsigned short const g_port_num =65001;

void send_auth();

void process_server_response(connection_ptr && soc, boost::system::error_code const & err)
{
	if (err && err != boost::asio::error::eof)
	{
		std::cerr << "Client Error on receive: " << err.message() << "\n";
		assert(false);
	}

	if (soc->data.size() != 2)
	{
		std::cerr << "Wrong bytes count\n";
		assert(false);
	}

	if (soc->data != "OK")
	{
		std::cerr << "Wrong response: " << soc->data << "\n";
		assert(false);
	}

	g_authed = true;
	soc->shutdown();
	tasks_processor::stop();
}

void receive_auth_response(connection_ptr && soc, boost::system::error_code const & err)
{
	if (err)
	{
		std::cerr << "Error on sending data: " << err.message() << "\n";
		assert(false);
	}

	async_read_data(std::move(soc), &process_server_response, 2);
}

void send_auth()
{
	connection_ptr soc = tasks_processor::create_connection("127.0.0.1", g_port_num);
	soc->data = "auth_name";
	async_write_data(std::move(soc), &receive_auth_response);
}


class authorizer
{
public:
	static void on_connection_accept(connection_ptr && connection, boost::system::error_code const & error)
	{
		if (error)
			return;

		async_read_data_at_least(std::move(connection), &authorizer::on_datareceive, 1, 1024);
	}

	static void on_datareceive(connection_ptr && connection, boost::system::error_code const & error)
	{
		if (error)
		{
			std::cerr << "authorizer.on_datareceive: error during receiving response: " << error << "\n";
			assert(false);
		}

		if (connection->data.size() == 0)
		{
			std::cerr << "authorizer.on_datareceive: zero bytes received\n";
			assert(false);
		}

		assert(connection->data == "auth_name");

		// do authorization
		connection->data = "OK";

		// send response
		async_write_data(std::move(connection), &authorizer::on_datasend);
	}

	static void on_datasend(connection_ptr && connection, boost::system::error_code const & error)
	{
		if (error)
		{
			std::cerr << "authorizer.on_datasend: error during sending response: " << "\n";
			assert(false);
		}

		connection->shutdown();
	}
};


int main(int argc, char * argv[])
{
	tasks_processor::run_delayed(boost::posix_time::seconds{1}, &send_auth);
	tasks_processor::add_listener(g_port_num, &authorizer::on_connection_accept);
	assert(!g_authed);

	tasks_processor::start();
	assert(g_authed);
}
