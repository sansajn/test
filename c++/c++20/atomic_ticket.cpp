/*! Fair ticketing with atomic notifications sample, taken from *C++20 The Complete Guide*, 
chapter 12, sample `lib/atomicticket.cpp` */
#include <queue>
#include <vector>
#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <semaphore>
#include <cassert>

using std::queue, std::vector;
using std::jthread, std::stop_token, std::mutex, std::lock_guard, std::atomic;
using std::cout;
namespace this_thread = std::this_thread;
using namespace std::literals;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	// initialize some values
	queue<char> values;
	mutex values_locker;
	for (int i = 0; i < 1000; ++i)
		values.push(static_cast<char>(i % (128 - 32) + 32));

	atomic<int> max_ticket{0},  // maximum requested ticket no
		act_ticket{0};  // current allowed ticket no

	// create and start pool of num_threads threads
	constexpr int num_threads = 10;
	vector<jthread> pool;
	for (int idx = 0; idx < num_threads; ++idx) {
		pool.push_back(jthread{
			[&, idx](stop_token stop) {
				while (!stop.stop_requested()) {
					// gen next value from the queue
					char val;
					{
						lock_guard lock{values_locker};
						assert(!empty(values));
						val = values.front();
						values.pop();
					}

					// request a ticket and wait until it is called
					int my_ticket{++max_ticket},
						act = act_ticket.load();

					while (act < my_ticket) {
						act_ticket.wait(act);
						act = act_ticket.load();
					}

					// print the value 10 times
					for (int i = 0; i < 10; ++i) {
						cout.put(val).flush();
						auto dur = 20ms * ((idx%3)+1);
						this_thread::sleep_for(dur);
					}

					// next ticket please
					++act_ticket;
					act_ticket.notify_all();
				}
			}
		});
	}

	// enable and disable threads in the thread pool
	auto enable = [&, old_num = 0](int new_num) mutable {
		act_ticket += new_num - old_num;
		if (new_num > 0)
			act_ticket.notify_all();
		old_num = new_num;
	};

	for (int num : {0, 3, 5, 2, 0, 1}) {
		cout << "\n\n====== enable " << num << " threaads" << std::endl;
		enable(num);
		this_thread::sleep_for(2s);
	}
	
	cout << "\n"
		<< "done!\n";

	std::quick_exit(0);  // this does not wait to finish threads
}
