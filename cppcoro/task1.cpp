// task sample from [C++20: Coroutines with cppcoro](https://www.modernescpp.com/index.php/c-20-coroutines-with-cppcoro) article
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

using std::chrono::high_resolution_clock,
	std::chrono::time_point,
	std::chrono::duration;
using std::cout, std::endl;
using namespace std::chrono_literals;

auto getTimeSince(time_point<high_resolution_clock> const & start)
{
	auto end = high_resolution_clock::now();
	duration<double> elapsed = end - start;
	return elapsed.count();
}

cppcoro::task<> third(time_point<high_resolution_clock> const & start)
{
	std::this_thread::sleep_for(1s);
	cout << "third waited " << getTimeSince(start) << " seconds." << endl;
	co_return;
}

cppcoro::task<> second(time_point<high_resolution_clock> const & start)
{
	auto thi = third(start);  // function call with immediate coroutine suspend (co_return called first)
	std::this_thread::sleep_for(1s);
	co_await thi;
	cout << "second waited " << getTimeSince(start) << " seconds." << endl;
}

cppcoro::task<> first(time_point<high_resolution_clock> const & start)
{
	auto sec = second(start);  // function call with immediate coroutine suspend (co_await called first)
	std::this_thread::sleep_for(1s);
	co_await sec;  // coroutine second "call"
	cout << "first waited " << getTimeSince(start) << " seconds." << endl;
}


int main(int argc, char * argv[])
{
	auto start = high_resolution_clock::now();
	cppcoro::sync_wait(first(start));
	cout << "main waited " << getTimeSince(start) << " seconds." << endl;
	cout << "done!\n";
	return 0;
}
