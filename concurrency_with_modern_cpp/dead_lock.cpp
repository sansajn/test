// deadlock sample (locking mutexes in a different order)
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

using std::mutex;
using std::thread;
using std::chrono::milliseconds;
namespace this_thread = std::this_thread;
using std::cout;

struct critical_data
{
	mutex locker;
};

void dead_lock(critical_data & a, critical_data & b)
{
	a.locker.lock();
	cout << "get the first mutex" << std::endl;
	this_thread::sleep_for(milliseconds(1));
	b.locker.lock();
	cout << "get the second mutex" << std::endl;
	a.locker.unlock();
	b.locker.unlock();
}

int main(int argc, char * argv[])
{
	critical_data c1, c2;

	thread t1{[&]{dead_lock(c1, c2);}};
	thread t2{[&]{dead_lock(c2, c1);}};

	t1.join();
	t2.join();

	return 0;
}
