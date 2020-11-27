// coroutine async_latch sample

#include <chrono>
#include <iostream>
#include <future>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/async_latch.hpp>
#include <cppcoro/task.hpp>

using std::cout, std::endl;
using namespace std::chrono_literals;

cppcoro::task<> wait_for(cppcoro::async_latch & latch)
{
	cout << "before co_await" << endl;
	co_await latch;
	cout << "after co_await" << endl;
}

int main(int argc, char * argv[])
{
	cppcoro::async_latch latch{3};

	auto waiter = std::async([&latch]{cppcoro::sync_wait(wait_for(latch));});

	auto counter1 = std::async([&latch]{
		std::this_thread::sleep_for(2s);
		cout << "counter1: latch.count_down() " << endl;
		latch.count_down();
	});

	auto counter2 = std::async([&latch]{
		std::this_thread::sleep_for(1s);
		cout << "counter2: latch.count_down(2) " << endl;
		latch.count_down(2);
	});

	waiter.get(), counter1.get(), counter2.get();

	cout << "done!\n";
	return 0;
}
