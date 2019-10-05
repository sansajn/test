/* fire and forget futures sample (note: task1 and task2 not running parallel
thanks to fire and forget futures (they join thread on its destructor)) */
#include <chrono>
#include <future>
#include <thread>
#include <iostream>

using namespace std::chrono;
using namespace std;

int main(int argc, char * argv[])
{
	async(launch::async, []{
		this_thread::sleep_for(seconds{2});
		cout << "first thread" << endl;
	});

	async(launch::async, []{
		this_thread::sleep_for(seconds{1});
		cout << "second thread" << endl;
	});

	cout << "main thread" << endl;

	return 0;
}
