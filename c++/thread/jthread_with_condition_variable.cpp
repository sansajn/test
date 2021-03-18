// jthread with condition variable sample
#include <thread>
#include <condition_variable>
#include <queue>
#include <iostream>
using std::jthread, std::stop_token, std::condition_variable_any, std::mutex, std::unique_lock;
using std::queue, std::size;
using std::cout, std::endl;
using namespace std::chrono_literals;

queue<int> jobs;
mutex locker;
condition_variable_any cv;

int main(int argc, char * argv[]) {
	jthread worker{[](stop_token stop){
		while (true) {
			int job = 0;

			{
				unique_lock lock{locker};
				if (!cv.wait(lock, stop, []{return !jobs.empty();}))
					break;

				job = jobs.front();
				jobs.pop();
			}

			// do something with job there ...
		}
	}};

	std::this_thread::sleep_for(100ms);

	// create some jobs
	for (int i = 1; i < 1000; ++i) {
		unique_lock lock{locker};
		jobs.push(i);
		cv.notify_one();
	}

	worker.request_stop();

	cout << "exiting with " << size(jobs) << " unexecuted jobs!\n";

	return 0;
}

