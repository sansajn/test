// spin_lock.cpp - spin lock implementation using atomic_flag

#include <atomic>
#include <thread>
#include <iostream>

using std::cout;

class spin_lock
{
public:
	void lock()
	{
		while (_flag.test_and_set())
			;
	}
	
	void unlock()
	{
		_flag.clear();
	}
	
private:
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};


spin_lock spin;

void work_on_resource()
{
	spin.lock();
	// do some work ...
	spin.unlock();
}


int main(int argc, char * argv[])
{
	std::thread t{work_on_resource};
	std::thread t2{work_on_resource};
	
	t.join();
	t2.join();
	
	cout << "done!\n";
	
	return 0;
}
