// making timers and processing timer events as tasks recipe
#include <memory>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/noncopyable.hpp>
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

protected:
	static boost::asio::io_context & get_io()
	{
		static boost::asio::io_context io;
		static boost::asio::io_context::work idle{io};
		return io;
	}
};  // task_processor

struct test_functor
{
	int & _i;
	explicit test_functor(int & i) : _i{i} {}

	void operator()() const
	{
		_i = 1;
		tasks_processor::stop();
	}
};

void test_func1()
{
	throw std::logic_error{"It works!"};
}

int main(int argc, char * argv[])
{
	constexpr int seconds_to_wait = 3;
	int i = 0;

	tasks_processor::run_delayed(
		boost::posix_time::seconds{seconds_to_wait},
		test_functor(i));

	tasks_processor::run_delayed(
		boost::posix_time::from_time_t(time(NULL) + 1),
		&test_func1);

	int t1 = static_cast<int>(time(NULL));
	assert(i == 0);

	// block till one of the tasks calls tasks_processor::stop()
	tasks_processor::start();

	assert(i == 1);
	int t2 = static_cast<int>(time(NULL));
	assert(t2 - t1 >= seconds_to_wait);

	return 0;
}
