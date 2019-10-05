// eager and lazy evaluation of a future

#include <chrono>
#include <future>
#include <iostream>

using namespace std::chrono;
using namespace std;

int main(int argc, char * argv[])
{
	auto begin = system_clock::now();

	auto async_lazy = async(launch::deferred, []{return system_clock::now();});
	auto async_eager = async(launch::async, []{return system_clock::now();});

	this_thread::sleep_for(seconds{1});

	auto lazy_start = async_lazy.get() - begin;
	auto eager_start = async_eager.get() - begin;

	cout << "async_lazy evaluated after: " << duration<double>(lazy_start).count() << " seconds\n"
		<< "async_eager evaluated after: " << duration<double>(eager_start).count() << " seconds"	<< endl;

	return 0;
}
