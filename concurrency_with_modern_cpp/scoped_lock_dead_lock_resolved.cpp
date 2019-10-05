// solving deadlock with unique_lock and lock() (delayed locking)
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

using std::chrono::milliseconds;
using namespace std;

struct critical_data
{
	mutex locker;
};

void dead_lock(critical_data & a, critical_data & b)
{
	cout << "thread: " << this_thread::get_id() << " first mutex " << endl;

	this_thread::sleep_for(milliseconds(1));

	cout << "thread: " << this_thread::get_id() << " second mutex " << endl;
	cout << "thread: " << this_thread::get_id() << " get both mutex" << endl;

	scoped_lock(a.locker, b.locker);
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
