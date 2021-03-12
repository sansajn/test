// nonblocking barrier implementation recipe
#include <iostream>
#include <boost/array.hpp>
#include <boost/atomic.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/deadline_timer.hpp>
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

using vector_type = boost::array<std::size_t, 10000>;
using data_t = boost::array<vector_type, 4>;

void fill_data(vector_type & data);
void compute_send_data(data_t & data);

void runner(size_t thread_index, boost::barrier & barrier, data_t & data)
{
	for (size_t i = 0; i < 1000; ++i)
	{
		fill_data(data.at(thread_index));
		barrier.wait();
		if (!thread_index)
			compute_send_data(data);
		barrier.wait();
	}
}

using atomic_count_t = boost::atomic<unsigned int>;

void clever_runner(size_t thread_index, size_t iteration, atomic_count_t & counter,
	data_t & data);

void clever_implementation()
{
	atomic_count_t counter{0};

	data_t data;

	for (size_t i = 0; i < data_t::static_size; ++i)
	{
		tasks_processor::push_task([i, &counter, &data]{
			clever_runner(i, 0, counter, data);
		});
	}

	tasks_processor::start();
}

void clever_runner(size_t thread_index, size_t iteration, atomic_count_t & counter,
	data_t & data)
{
	fill_data(data.at(thread_index));

	if (++counter != data_t::static_size)
		return;

	compute_send_data(data);

	if (++iteration == 1000)
	{
		// exit,because 1000 iterationsare done
		tasks_processor::stop();
		return;
	}

	counter = 0;
	for (size_t i = 0; i < data_t::static_size; ++i)
	{
		tasks_processor::push_task([i, iteration, &counter, &data]{
			clever_runner(i, iteration, counter, data);
		});
	}
}

int main(int argc, char * argv[])
{
	boost::barrier barrier{data_t::static_size};

	data_t data;

	boost::thread_group tg;
	for (size_t i = 0; i < data_t::static_size; ++i)
	{
		tg.create_thread([i, &barrier,&data]{
			runner(i, barrier, data);
		});
	}

	tg.join_all();

	clever_implementation();

	return 0;
}

void fill_data(vector_type & data)
{
	for (size_t i = 0; i < vector_type::static_size; ++i)
		data[i] = i;
}

void compute_send_data(data_t & data)
{
	for (size_t i = 0; i < vector_type::static_size; ++i)
	{
		for (size_t j = 0; j < data_t::static_size; ++j)
		{
			assert(data[j][i] == i);
			data[0][i] *= data[j][i];
		}
	}
}
