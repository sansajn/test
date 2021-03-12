// conveyor tasks processing recipe
#include <memory>
#include <algorithm>
#include <deque>
#include <iostream>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/atomic/atomic.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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

protected:
	static boost::asio::io_context & get_io()
	{
		static boost::asio::io_context io;
		static boost::asio::io_context::work idle{io};
		return io;
	}
};  // task_processor

class work_queue
{
public:
	using task_type = boost::function<void()>;

	work_queue() : _is_stopped{false} {}

	void run()
	{
		while (1)
		{
			boost::unique_lock<boost::mutex> lock{_mutex};
			while (_tasks.empty())
			{
				if (_is_stopped)
					return;
				_cond.wait(lock);
			}

			task_type t = std::move(_tasks.front());
			_tasks.pop_front();
			lock.unlock();

			t();
		}
	}

	void stop()
	{
		boost::lock_guard<boost::mutex> lock{_mutex};
		_is_stopped = true;
		_cond.notify_all();
	}

	void push_task(task_type && task)
	{
		boost::unique_lock<boost::mutex> lock{_mutex};
		if (_is_stopped)
			return;

		_tasks.push_back(std::move(task));

		lock.unlock();
		_cond.notify_one();
	}

private:
	std::deque<task_type> _tasks;
	boost::mutex _mutex;
	boost::condition_variable _cond;
	bool _is_stopped;
};


using atomic_t = boost::atomic<unsigned int>;

struct data_packet {unsigned int value;};
struct decoded_data {unsigned int value;};
struct compressed_data {unsigned int value;};

class subsystem1 : private boost::noncopyable
{
public:
	subsystem1() : _i{0} {}

	data_packet get_data() {return data_packet{++_i};}

	bool is_stopped() const {return _i == max_runs;}

	static const unsigned int max_runs = 10000;

private:
	atomic_t _i;
};

class subsystem2 : private boost::noncopyable
{
public:
	subsystem2() : _i{0} {}

	void send_data(compressed_data const & data) {++_i; assert(data.value == _i);}

	unsigned int send_packet_count() const {return _i;}

private:
	atomic_t _i;
};

subsystem1 subs1;
subsystem2 subs2;

decoded_data decode_data(data_packet const & packet);
compressed_data compress_data(decoded_data const & packet);

void process_data()
{
	while (!subs1.is_stopped())
	{
		data_packet data = subs1.get_data();
		decoded_data d_decoded = decode_data(data);
		compressed_data c_data = compress_data(d_decoded);
		subs2.send_data(c_data);
	}
}

void run_in_multiple_threads()
{
	boost::thread t{&process_data};
	process_data();
	t.join();
}

work_queue decoding_queue, compressing_queue, sending_queue;

void start_data_accepting();
void do_decode(data_packet const & packet);
void do_compress(decoded_data const & packet);

void start_data_accepting()
{
	while (!subs1.is_stopped())
	{
		data_packet packet = subs1.get_data();
		decoding_queue.push_task([packet]{
			do_decode(packet);});
	}
}

void do_decode(data_packet const & packet)
{
	decoded_data d_decoded = decode_data(packet);
	compressing_queue.push_task([d_decoded]{
		do_compress(d_decoded);
	});
};

void do_compress(decoded_data const & packet)
{
	compressed_data c_data = compress_data(packet);
	sending_queue.push_task([c_data]{  // TODO: are data moved or copied here ?
		subs2.send_data(c_data);
	});
}


int main(int argc, char * argv[])
{
	boost::thread t_data_decoding{[]{
		decoding_queue.run();
	}};

	boost::thread t_data_compressing{[]{
		compressing_queue.run();
	}};

	boost::thread t_data_sending{[]{
		sending_queue.run();
	}};

	start_data_accepting();

	decoding_queue.stop();
	t_data_decoding.join();

	compressing_queue.stop();
	t_data_compressing.join();

	sending_queue.stop();
	t_data_sending.join();

	assert(subs2.send_packet_count() == subsystem1::max_runs);

	return 0;
}

decoded_data decode_data(data_packet const & packet)
{
	static unsigned int i = 0;
	++i;
	decoded_data ret = {packet.value};
	assert(i == packet.value);
	return ret;
}

compressed_data compress_data(decoded_data const & packet)
{
	static unsigned int i = 0;
	++i;
	compressed_data ret = {packet.value};
	assert(i == packet.value);
	return ret;
}
