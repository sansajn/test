/*! atomic_ref sample taken from *C++20 The Complete Guide* book, 
chapter 12, sample `lib/atomicref.cpp`. */
#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <fmt/format.h>
#include <random>
#include <thread>
#include <atomic>

using std::array, std::vector;
using std::fill_n;
using std::jthread, std::stop_source, std::stop_token;
using std::atomic_ref;
using std::mt19937, std::uniform_int_distribution, std::random_device;
using std::cout, fmt::format;
using namespace std::literals;  // for duration literals
namespace this_thread = std::this_thread; 


int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	// create and initialize array of values
	array<int, 1000> values;
	fill_n(begin(values), size(values), 100);

	// initialize a common stop token for all threads
	stop_source all_stop_source;
	stop_token all_stop_token{all_stop_source.get_token()};

	// start multiple treads concurrently decrementing the value
	vector<jthread> threads;
	for (int i = 0; i < 9; ++i) {
		threads.push_back(jthread{
			[&values](stop_token stop) {
				// initialize random engine to generate an index
				mt19937 reng{random_device{}()};
				uniform_int_distribution rdistr{0, static_cast<int>(size(values)-1)};

				while (!stop.stop_requested()) {
					// compute the next index
					int idx = rdistr(reng);

					// enable atomic access to the value with the index
					atomic_ref val{values[idx]};

					// and use it
					--val;
					if (val <= 0)
						cout << format("index {} is zero\n", idx);
				}
			}, 
			all_stop_token  // pass the common stop token
		});
	}

	// after a while/event request to stop all threads
	this_thread::sleep_for(0.5s);
	cout << "\nSTOP\n";
	all_stop_source.request_stop();

	return 0;
}
