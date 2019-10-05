// creation of a thread with callable units
#include <thread>
#include <iostream>

using std::thread;
using std::cout;

void hello_function()
{
	cout << "hello from a function" << std::endl;
}

class hello_function_object
{
public:
	void operator()() const
	{
		cout << "hello from a function object"	<< std::endl;
	}
};

int main(int argc, char * argv[])
{
	thread t1{hello_function};

	hello_function_object obj;
	thread t2{obj};

	thread t3{[]{cout << "hello from a lambda" << std::endl;}};

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
