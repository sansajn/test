// coroutine producer consumer sample from [C++20: Powerful Coroutines with cppcoro](https://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro) article

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/single_consumer_event.hpp>

using std::chrono::high_resolution_clock,
	std::chrono::duration;
using namespace std::chrono_literals;
using std::cout, std::endl;

cppcoro::single_consumer_event __event;

cppcoro::task<> consumer() 
{
	auto start = high_resolution_clock::now();

	co_await __event;  // suspend until some thread calls __event.set()

	auto end = high_resolution_clock::now();
	duration<double> elapsed = end - start;
	cout << "consumer waited " << elapsed.count() << " seconds." << endl;

	co_return;
}

void producer()  // is not coroutine
{
	std::this_thread::sleep_for(2s);
	__event.set();  // resumes the consumer
}

int main(int argc, char * argv[])
{
	auto con = std::async([]{cppcoro::sync_wait(consumer());});
	auto prod = std::async(producer);

	con.get(), prod.get();  // wait for threads to finish

	cout << "done!\n";
	return 0;
}
