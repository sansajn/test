// storing an exception andmaking a task from it section recipe
#include <stdexcept>
#include <iostream>
#include <boost/array.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/exception_ptr.hpp>
#include <boost/lexical_cast.hpp>

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

struct process_exception
{
	explicit process_exception(boost::exception_ptr const & exc)
		: _exc{exc}
	{}

	void operator()() const;

	boost::exception_ptr _exc;
};

void func_test2();

void process_exception::operator()() const
{
	try {
		boost::rethrow_exception(_exc);
	}
	catch (boost::bad_lexical_cast &)
	{
		std::cout << "Lexical cast exception detected.\n" << std::endl;

		tasks_processor::push_task(&func_test2);
	}
	catch (...)
	{
		std::cout << "Can not handle such exceptions:\n"
			<< boost::current_exception_diagnostic_information()
			<< std::endl;

		tasks_processor::stop();
	}
}

void func_test1()
{
	try {
		boost::lexical_cast<int>("oops!");
	}
	catch (...)
	{
		tasks_processor::push_task(
			process_exception(boost::current_exception()));
	}
}

void func_test2()
{
	try {
		BOOST_THROW_EXCEPTION(std::logic_error{"Some fatal logic error"});
	}
	catch (...)
	{
		tasks_processor::push_task(
			process_exception(boost::current_exception()));
	}
}

void run_throw(boost::exception_ptr & ptr)
{
	try {
		// a lot of code goes there ...
	}
	catch (...)
	{
		ptr = boost::current_exception();
	}
}


int main(int argc, char * argv[])
{
	tasks_processor::push_task(&func_test1);
	tasks_processor::start();

	boost::exception_ptr ptr;

	// do some work in parallel
	boost::thread t{&run_throw, boost::ref(ptr)};

	// some code goes here
	// ...

	t.join();

	// checking for exception
	if (ptr)
	{
		// exception occured in thread
		boost::rethrow_exception(ptr);
	}

	return 0;
}
