// taking a future with a shared_future
#include <future>
#include <iostream>
#include <thread>
#include <utility>

using namespace std;

mutex cout_locker;

struct div_operation
{
	void operator()(promise<int> && p, int a, int b)
	{
		p.set_value(a/b);
	}
};

struct requestor
{
	void operator()(shared_future<int> fut)
	{
		lock_guard<mutex> cout_lock{cout_locker};
		cout << "thread_id=" << this_thread::get_id() << ": " << "20/10=" << fut.get() << endl;
	}
};

int main(int argc, char * argv[])
{
	// define promises
	promise<int> div_promise;

	// get futures
	shared_future<int> div_result = div_promise.get_future();

	// calculate the result	in a separat thread
	thread div_thread{div_operation{}, move(div_promise), 20, 10};

	thread shared_thread1{requestor{}, div_result};
	thread shared_thread2{requestor{}, div_result};
	thread shared_thread3{requestor{}, div_result};
	thread shared_thread4{requestor{}, div_result};
	thread shared_thread5{requestor{}, div_result};

	div_thread.join();

	shared_thread1.join();
	shared_thread2.join();
	shared_thread3.join();
	shared_thread4.join();
	shared_thread5.join();

	return 0;
}
