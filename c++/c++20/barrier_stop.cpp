/* barrier stop sample taken from *C++20 The Complete Guide*, sample `lib/barrierstop.cpp` */
#include <vector>
#include <thread>
#include <barrier>
#include <iostream>
#include <fmt/format.h>
using std::vector;
using std::cout, fmt::format;
using std::barrier, std::jthread, std::stop_token;
namespace this_thread = std::this_thread;
using namespace std::literals;  // for duration literals

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	vector values{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

	auto print_values = [&values]() noexcept {
		for (auto const & val : values)
			cout << format(" {:<7.5}", val);
		cout << '\n';
	};

	print_values();

	// initialize a barrier that prints the values when all threads have done their computations
	barrier all_done{static_cast<int>(size(values)),  // initialal barrier value
		print_values};  // callback to call whenever the counter is 0

	// initialie a thread for each value to compute its square rot in a loop
	vector<jthread> pool;
	for (size_t idx = 0; idx < size(values); ++idx) {
		pool.push_back(jthread{[idx, &values, &all_done](stop_token stop){
			while (!stop.stop_requested()) {
				values[idx] = sqrt(values[idx]);
				this_thread::sleep_for(10ms * idx);
				all_done.arrive_and_wait();  // singronize with other threads to print values
			}

			// remove thread from counter so that other threads don't wait
			all_done.arrive_and_drop();
		}});
	}

	this_thread::sleep_for(0.5s);
	cout << "\nSTOP\n";
	pool.clear();

	return 0;
}
