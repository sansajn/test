#include <mutex>
#include <thread>
#include <iostream>

using std::lock_guard;
using std::mutex;
using std::thread;
using std::cout;


void loop(mutex * m)
{
	while (1)
	{
		lock_guard<mutex> lock{*m};
		cout << " Peter\n" << std::endl;
	}
}


int main(int argc, char * argv[]) 
{
	mutex m;
	thread t{loop, &m};

	while (1)
	{
		lock_guard<mutex> lock{m};
		cout << "hi";
	}

	return 0;
}
