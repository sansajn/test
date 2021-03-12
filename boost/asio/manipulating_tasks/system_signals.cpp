// storing an exception andmaking a task from it section recipe
#include <stdexcept>
#include <iostream>
#include <boost/array.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/signal_set.hpp>
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
};  // task_processor

void accept_3_signals_and_stop(int signal)
{
	static int signals_count = 0;
	assert(signal == SIGINT);

	++signals_count;
	std::cout << "Captured " << signals_count << " SIGINT\n";
	if (signals_count == 3)
		tasks_processor::stop();
}

int main(int argc, char * argv[])
{
	tasks_processor::register_signals_handler(
		&accept_3_signals_and_stop, {SIGINT, SIGSEGV});

	tasks_processor::start();

	return 0;
}
