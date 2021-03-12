// registering a task for processing anarbitrary datatype recipe
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/noncopyable.hpp>

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

protected:
	static boost::asio::io_context & get_io()
	{
		static boost::asio::io_context io;
		static boost::asio::io_context::work idle{io};
		return io;
	}
};  // task_processor

int g_val = 0;
void func_test()
{
	++g_val;
	if (g_val == 3)
		throw std::logic_error{"Just checking"};

	boost::this_thread::interruption_point();
	if (g_val == 10)
	{
		// emulation of thread interruption
		// will be cauht and won't stop execution.
		throw boost::thread_interrupted{};
	}

	if (g_val == 90)
		tasks_processor::stop();
}


int main(int argc, char * argv[])
{
	constexpr size_t tasks_count = 100;

	// stop() is called at 90
	BOOST_STATIC_ASSERT(tasks_count > 90);
	for (size_t i = 0; i < tasks_count; ++i)
		tasks_processor::push_task(&func_test);

	// bind can be used also
	tasks_processor::push_task(boost::bind(std::plus<int>{}, 2, 2));

	assert(g_val == 0);  // process was not started

	tasks_processor::start();
	assert(g_val == 90);

	return 0;
}
