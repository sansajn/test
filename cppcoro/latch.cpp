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
	return 0;
}

