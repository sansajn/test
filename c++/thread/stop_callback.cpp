// stop_source usage with thread sample 
#include <stop_token>
#include <thread>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <iostream>

using std::stop_source, std::stop_token, std::stop_callback, std::thread;
using std::condition_variable, std::mutex, std::unique_lock;
using std::queue;
using std::cout, std::endl, std::flush;
using namespace std::chrono_literals;

queue<int> jobs;
mutex locker;
condition_variable cv;

void worker(stop_token && token)
{
	stop_callback cb{token, []{
		cv.notify_all();
	}};

	while (true)
	{
		int job_id = -1;

		{  // acquire a job ID in lock
			unique_lock lock{locker};
			cv.wait(lock, [token]{
				return !jobs.empty() || token.stop_requested();
			});

			if (token.stop_requested())
				break;

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
	cout << "done\n";
	return 0;
}
