/* Binary semaphore sample taken from *C++20 The Complete Guide*, 
chapter 12 *Concurency Features*, `lib/semaphorenotify.cpp`. */
#include <iostream>
#include <chrono>
#include <thread>
#include <semaphore>

using std::jthread, std::stop_token;
using std::binary_semaphore;
using std::cout;
namespace this_thread = std::this_thread;
using namespace std::literals;  // for duration literals

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	int shared_data;
	binary_semaphore data_ready{0},
		data_done{0};

	jthread process{[&](stop_token stop){
		while (!stop.stop_requested()) {
			// wait until next value ready with 1s timeout to check stop_token
			if (data_ready.try_acquire_for(1s)) {
				int data = shared_data;

				// process it
				cout << "[process] read " << data << std::endl;
				this_thread::sleep_for(data * .5s);
				cout << "[process]\tdone" << std::endl;

				// signal processing done
				data_done.release();
			}
			else
				cout << "[process] timeout" << std::endl;
		}
	}};

	// generate a couple of values
	for (int i = 0; i < 10; ++i) {
		// store next value
		cout << "[main] store " << i << std::endl;
		shared_data = i;

		// signal to start processing
		data_ready.release();

		// wait until processing is done
		data_done.acquire();

		cout << "[main] processing done\n" << std::endl;
	} 
	
	return 0;
}
