// coroutine async_mutex sample from [C++20: Powerful Coroutines with cppcoro](https://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro) article

#include <thread>
#include <vector> 
#include <iostream>
#include <cppcoro/async_mutex.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>


using std::vector, std::thread, std::cout, std::endl;

cppcoro::async_mutex __locker;

int __sum = 0;

cppcoro::task<> add_to_sum(int n)
{
	cppcoro::async_mutex_lock lock = co_await __locker.scoped_lock_async();
	__sum += n;
}

int main(int argc, char * argv[])
{
	vector<thread> v(10);

	for (auto & t : v)
	{
		t = thread([]{
			for (int n = 0; n < 10; ++n)
				cppcoro::sync_wait(add_to_sum(n));
		});
	}

	for (auto & t : v)
		t.join();

	cout << "sum: " << __sum << "\n";

	cout << "done!\n";
	return 0;
}
