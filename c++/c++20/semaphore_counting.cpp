/* Counting semaphore sample taken from *C++20 The Complete Guide*, 
chapter 12 *Concurency Features*, `lib/semaphore.cpp`. */
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore>

using std::jthread, std::stop_token;
using std::mutex, std::lock_guard, std::counting_semaphore;
using std::queue, std::vector;
using std::move;
using std::cout;
namespace this_thread = std::this_thread;
using namespace std::literals;  // for duration literals

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	queue<char> values;
	mutex values_locker;

	for (int i = 0; i < 1000; ++i)
		values.push(static_cast<char>('a' + (i % ('z' - 'a'))));

	// create a pool of num_threads threads and 
	// limit their availability with a semaphore (initially none available)
	constexpr int num_threads = 10;
	counting_semaphore<num_threads> enabled{0};

	// create and start all threads of the pool
	vector<jthread> pool;
	for (int idx = 0; idx < num_threads; ++idx) {
		jthread t{[idx, &enabled, &values, &values_locker](stop_token st) {
			while (!st.stop_requested()) {
				enabled.acquire();  // requires thread to become one of the enabled threads

				// get next value from the queue
				char val;
				{
					lock_guard lock{values_locker};
					val = values.front();
					values.pop();
				}

				// print the value 10 times
				for (int i = 0; i < 10; ++i) {
					cout.put(val).flush();
					auto dur = 130ms * ((idx%3) + 1);
					this_thread::sleep_for(dur);
				}

				// remove thread from the set ofenabled threads
				enabled.release();
			}
		}};

		pool.push_back(move(t));
	}

	cout << "== wait 2 seconds (no thread enabled)\n" << std::flush;
	this_thread::sleep_for(2s);

	// enable 3 concurrent threads
	cout << "== enable 3 parallel threads\n" << std::flush;
	enabled.release(3);
	this_thread::sleep_for(2s);

	// enable 2 more concurrent threads
	cout << "\n== enable 2 more parallel threads\n" << std::flush;
	enabled.release(2);
	this_thread::sleep_for(2s);

	// normally we want run forever, but let's end the program there
	cout << "\n== stop processing\n" << std::flush;
	for (auto & t : pool)
		t.request_stop();

	return 0;
}
