// condition variable use case (sender/receiver) sample

#include <condition_variable>
#include <iostream>
#include <thread>

using std::thread;
using std::mutex, std::condition_variable;
using std::unique_lock, std::lock_guard;
using std::cout, std::endl;

mutex data_locker;
condition_variable data_sync;

bool data_ready = false;

void waiting_for_work()
{
	cout << "waiting " << endl;
	unique_lock<mutex> lock{data_locker};
	data_sync.wait(lock, []{return data_ready;});
	cout << "running " << endl;
}

void set_data_ready()
{
	{
		lock_guard<mutex> lock{data_locker};
		data_ready = true;
	}

	cout << "data prepared" << endl;
	
	data_sync.notify_one();
}

int main(int argc, char * argv[])
{
	thread t1{waiting_for_work},
		t2{set_data_ready};

	t1.join();
	t2.join();

	cout << "done.\n";

	return 0;
}
