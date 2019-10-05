// condition_variable sample use
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

using std::vector;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;
using std::thread;
using std::cout;

vector<int> _shared_work;
mutex _locker;
condition_variable _cond;

bool _data_ready = false;  // modification needs to be mutex protected (even in a case _data_ready is atomic), otherwise race condition with condition_variable

void waiting_for_work()
{
	cout << "waiting" << std::endl;
	unique_lock<mutex> lock{_locker};
	_cond.wait(lock, []{return _data_ready;});
	_shared_work[1] = 2;
	cout << "work done" << std::endl;
}

void set_data_ready()
{
	_shared_work = {1, 0, 3};
	{
		lock_guard<mutex> lock{_locker};
		_data_ready = true;
	}
	cout << "data prepared" << std::endl;
	_cond.notify_one();
}

int main(int argc, char * argv[])
{
	cout << std::endl;

	thread t1{waiting_for_work};
	thread t2{set_data_ready};

	t1.join();
	t2.join();

	for (auto v : _shared_work)
		cout << v << " ";

	cout << "\ndone!";

	return 0;
}
