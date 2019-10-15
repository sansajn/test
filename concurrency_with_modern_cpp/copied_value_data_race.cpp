// data races with references
#include <functional>
#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono_literals;
using namespace std;

void by_copy(bool b)
{
	this_thread::sleep_for(1ms);
	cout << "by_copy: " << b << endl;
}

void by_reference(bool & b)
{
	this_thread::sleep_for(1ms);
	cout << "by_reference: " << b << endl;
}

void by_const_reference(bool const & b)
{
	this_thread::sleep_for(1ms);
	cout << "by_const_reference: " << b << endl;
}


int main(int argc, char * argv[])
{
	cout << boolalpha;

	bool shared = false;

	thread t1{by_copy, shared};
	thread t2{by_reference, ref(shared)};
	thread t3{by_const_reference, cref(shared)};

	shared = true;

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
