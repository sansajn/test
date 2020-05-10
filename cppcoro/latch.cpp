// latch sample
#include <chrono>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/async_latch.hpp>
#include <cppcoro/task.hpp>

using namespace std::chrono_literals;

using cppcoro::async_latch, cppcoro::task;

task<> wait_for(async_latch & latch)
{
	cout << "before co_wait" << endl;
	co_await async_latch;
	cout << "after co_await" << endl;
}

int main(int argc, char * argv[])
{
	async_latch latch{3};
	auto waiter = async([&latch]{sync_wait(wait_for(latch));});

	auto counter1 = async([&latch]{
		this_thread::sleep_for(2s));
		cout << "counter 1: latch.count_down()" << endl;
		latch.count_down();
	});

	auto counter2 = async([&latch]{
		this_thread::sleep_for(1s);
		cout << "counter 2: latch.count_down(2)" << endl;
		latch.count_down(2);
	});

	waiter.get();
	counter1.get();
	counter2.get();

	return 0;
}

