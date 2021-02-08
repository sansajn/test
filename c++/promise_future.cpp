// synchronization via promise/future sample
#include <future>
#include <thread>
#include <functional>
#include <chrono>
#include <iostream>

using std::promise;
using std::thread;
using std::ref;
using std::cout;
using namespace std::chrono_literals;


void worker(promise<int> & result)
{
	std::this_thread::sleep_for(100ms);
	result.set_value(42);
}


int main(int argc, char * argv[])
{
	promise<int> result_promise;

	thread worker_thread{worker, ref(result_promise)};

	// wait for worker result
	int result = result_promise.get_future().get();
	cout << "the answer is " << result << "\n";

	worker_thread.join();

	cout << "done!\n";
	return 0;
}
