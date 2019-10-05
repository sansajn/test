// local_thread sample, state between function calls

#include <iostream>
#include <string>
#include <mutex>
#include <thread>

using namespace std;

thread_local string _s{"hello from "};

mutex cout_locker;

void first()
{
	_s += "first ";
}

void second()
{
	_s += "second ";
}

void third()
{
	_s += "third ";
}

void add_thread_local(string const & s)
{
	_s += s;

	first();
	second();
	third();

	lock_guard<mutex> lock{cout_locker};
	cout << s << "\n"
		<< "&s:" << &s << "\n"
		<< endl;
}

int main(int argc, char * argv[])
{
	cout << endl;

	thread t1{add_thread_local, "t1: "};
	thread t2{add_thread_local, "t2: "};
	thread t3{add_thread_local, "t3: "};
	thread t4{add_thread_local, "t4: "};

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}
