// thread pool sample
#include <chrono>
#include <string>
#include <iostream>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/static_thread_pool.hpp>
#include <cppcoro/when_all.hpp>

using std::chrono::duration_cast, std::chrono::steady_clock;
using namespace std::chrono_literals;
using std::string;
using std::cout, std::endl;

cppcoro::task<string> first()
{
	this_thread::sleep_for(1s);
	co_return "First";
}

cppcoro::task<string> second()
{
	this_thread::sleep_for(1s);
	co_return "Second";
}

cppcoro::task<string> third()
{
	this_thread::sleep_for(1s);
	co_return "Third";
}

template <typename Func>
cppcoro::task<string> run_on_thread_pool(cppcoro::static_thread_pool & tp, Func func)
{
	co_await tp.schedule();
	auto res = co_await func();
	co_return res;
}

cppcoro::task<> run_all()
{
	auto [fir, sec, thi] = co_await cppcoro::when_all(
		run_on_thread_pool(tp, first), 
		run_on_thread_pool(tp, second),
		run_on_thread_pool(tp, third));
	cout << fir << " " << sec << " " << thi << endl;
}

int main(int argc, char * argv[])
{
	auto start = steady_clock::now();
	cppcoro::static_thread_pool tp;
	cppcoro::sync_wait(run_all(tp));
	cout << "execution time " << duration_cast<double>(steady_clock::now() - start).count() << "s\n";
	return 0;
}

