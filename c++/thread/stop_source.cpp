// stop_source usage with thread sample 
#include <stop_token>
#include <thread>
#include <chrono>
#include <iostream>

using std::stop_source, std::stop_token, std::thread;
using std::cout, std::endl, std::flush;
using namespace std::chrono_literals;

void job(stop_token token)
{
	while (!token.stop_requested())
	{
		// do something usefull
		std::this_thread::sleep_for(1s);
		cout << "*";
		flush(cout);
	}
}

int main(int argc, char * argv[])
{
	stop_source source;

	thread processor1{job, source.get_token()},
		processor2{job, source.get_token()};

	std::this_thread::sleep_for(5s);

	source.request_stop();

	processor1.join();
	processor2.join();

	cout << "\ndone!\n";
	return 0;
}
