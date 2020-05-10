// mutex sample
#include <vector>
#include <thread>
#include <iostream>
#include <cppcoro/async_mutex.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

using std::vector;
using std::thread;
using std::cout;

cppcoro::async_mutex_lock sum_lock;

int sum = 0;

cppcoro::task<> add_to_sum(int num)
{
	cppcoro::async_mutex_lock lock = co_await sum_lock.scoped_lock_async();
	sum += num;
}

int main(int argc, char * argv[])
{
	vector<thread> vec(10);
	for (auto & t : vec)
		t = thread([]{
			for (int n = 0; n < 10; ++n)
				cppcoro::sync_wait(add_to_sum(n));
			});

	for (auto & t : vec) 
		t.join();

	cout << "sum = " << sum << "\n"
		<< "done.\n";

	return 0;
}
