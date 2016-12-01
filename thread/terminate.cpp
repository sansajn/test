#include <thread>
#include <exception>
#include <iostream>

using std::cout;

void thread_func()
{
	while (true)
		;
}

int main(int argc, char * argv[])
{
	std::thread t{thread_func};
	std::this_thread::sleep_for(std::chrono::seconds{3});
	cout << "terminating ..." << std::endl;
//	std::terminate();
	exit(0);
	cout << "done" << std::endl;
	return 0;
}
