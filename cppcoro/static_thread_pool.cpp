// static_thread_pool sample (see when_all for a version without thread pool)

#include <chrono>
#include <thread>
#include <iostream>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/static_thread_pool.hpp>
#include <cppcoro/when_all.hpp>

using std::chrono::steady_clock, std::chrono::duration;
using std::string;
using std::cout, std::endl;
using namespace std::chrono_literals;

cppcoro::task<string> get_first()
{
	std::this_thread::sleep_for(1s);
	cout << "first from thread=" << std::this_thread::get_id() << "\n";
	co_return "first";
}

cppcoro::task<string> get_second()
{
	std::this_thread::sleep_for(1s);
	cout << "second from thread=" << std::this_thread::get_id() << "\n";
	co_return "second";
}

cppcoro::task<string> get_third()
{
	std::this_thread::sleep_for(1s);
	cout << "third from thread=" << std::this_thread::get_id() << "\n";
	co_return "third";
}

template <typename Func>
cppcoro::task<string> run_on_thread_pool(cppcoro::static_thread_pool & tp, Func func)
{
	co_await tp.schedule();
	auto res = co_await func();
	co_return res;  // can I do `co_return co_await func();` ?
}

cppcoro::task<> run_all(cppcoro::static_thread_pool & tp)
{
	cout << "run_all from thread=" << std::this_thread::get_id() << "\n";

	auto [first, second, third] = co_await cppcoro::when_all(
		run_on_thread_pool(tp, get_first), 
		run_on_thread_pool(tp, get_second),
		run_on_thread_pool(tp, get_third));

	cout << first << " " << second << " " << third << endl;
}

int main(int argc, char * argv[])
{
	auto start = steady_clock::now();

	cout << "main thread=" << std::this_thread::get_id() << "\n";

	cppcoro::static_thread_pool tp;
	cppcoro::sync_wait(run_all(tp));

	auto end = steady_clock::now();
	duration<double> elapsed = end - start;
	cout << "execution time " << elapsed.count() << "s\n";

	cout << "done!\n";
	return 0;
}
