// call_once sample

#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

once_flag once;

void do_once()
{
	call_once(once, []{cout << "Only once." << endl;});
}

void do_once2()
{
	call_once(once, []{cout << "Only once2." << endl;});
}

int main()
{
	cout << endl;

	thread t1{do_once};
	thread t2{do_once};
	thread t3{do_once2};
	thread t4{do_once2};

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	cout << endl;

	return 0;
}
