// condition_variable_any with stop token sample
#include <stop_token>
#include <thread>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <iostream>

using std::stop_source, std::stop_token, std::stop_callback, std::thread;
using std::condition_variable_any, std::mutex, std::unique_lock;
using std::queue;
using std::cout, std::endl, std::flush;
using namespace std::chrono_literals;

queue<int> jobs;
mutex locker;
condition_variable_any cv;

void worker(stop_token token)
{
	while (true)
	{
		int job_id = -1;

		{  // acquire a job ID in lock
			unique_lock lock{locker};
			if (!cv.wait(lock, token, []{return !jobs.empty();}))
				break;  // stop request

			job_id = jobs.front();
			jobs.pop();
		}

		// do job
		cout << std::this_thread::get_id() << " doing job " << job_id << endl;
	}
}

void manager()
{
	stop_source source;

	thread w1{worker, source.get_token()},
		w2{worker, source.get_token()};

	for (int i = 0; i < 5; ++i)
	{
		unique_lock lock{locker};
		jobs.push(i);
		cv.notify_one();  // wakes up only one worker
	}

	std::this_thread::sleep_for(1s);

	source.request_stop();

	w1.join();
	w2.join();
}

int main(int argc, char * argv[])
{
	manager();
	cout << "done!\n";
	return 0;
}
