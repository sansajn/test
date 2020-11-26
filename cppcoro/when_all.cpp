// waitinch for coroutines with when_all sample

#include <chrono>
#include <iostream>
#include <thread>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
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

cppcoro::task<> run_all()
{
	auto [first, second, third] = co_await cppcoro::when_all(get_first(), get_second(), get_third());
	cout << first << " " << second << " " << third << endl;
}

int main(int argc, char * argv[])
{
	auto start = steady_clock::now();

	cppcoro::sync_wait(run_all());

	auto end = steady_clock::now();
	duration<double> elapsed = end - start;
	cout << "execution time " << elapsed.count() << "s\n";

	cout << "done!\n";
	return 0;
}
