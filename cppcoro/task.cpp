//! cppcoro task sample
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

using std::cout, std::endl;
using std::chrono::time_point, 
	std::chrono::high_resolution_clock,
	std::chrono::duration_cast;

double time_since(time_point<high_resolution_clock> const & t)
{
	return duration_cast<double>(high_resolution_clock::now() - t).count();
}

cppcoro::task<> third(time_point<high_resolution_clock> const & start)
{
	this_thread::sleep_for(1s);
	cout << "third waited " << time_since(start) << "s." << endl;
	co_return;
}

cppcoro::task<> second(time_point<high_resolution_clock> const & start)
{
	cppcoro::task<> thi = third(start);
	this_thread::sleep_for(1s);
	co_await thi;
	cout << "second waited " << time_since(start) << "s." << endl;
}

cppcoro::task<> first(time_point<high_resolution_clock> const & start)
{
	cppcoro::task<> sec = second(start);
	this_thread::sleep_for(1s);
	co_await sec;
	cout << "first waited " << time_since(start) << "s." << endl;
}

int main(int argc, char * argv[])
{
	auto start = high_resolution_clock::now();
	cppcoro::sync_wait(first(start));
	cout << "main waited " << time_since(start) << "s." << endl;
	cout << "done.\n";
	return 0;
}

