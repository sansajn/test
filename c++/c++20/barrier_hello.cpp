// barrier (`barrier.cpp`) sample from *C++20 - The Cpmplete Guide* book,  chapter 12.1.2
#include <iostream>
#include <fmt/format.h>  // <format> replacement
#include <thread>
#include <vector>
#include <cmath>
#include <barrier>
using std::cout, std::barrier, std::jthread, std::vector;
using fmt::format;  // std::format replacement

int main(int argc, char * argv[]) {
	vector values{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
	
	// NOTE: has to be noexcept to be used as barrier callback
	auto printValues = [&values]() noexcept {
		for (auto val : values)
			cout << format(" {:<7.5}", val);
		cout << '\n';
	};

	printValues();  // print initial values

	barrier allDone{static_cast<ptrdiff_t>(size(values)),   // initial counter value
		printValues};  // callbackto call whenever the counter reach 0

	vector<jthread> threads;
	for (size_t idx = 0; idx < size(values); ++idx) {
		threads.push_back(jthread{
			[idx, &values, &allDone] {
				for (int i = 0; i < 5; ++i) {
					values[idx] = sqrt(values[idx]);
					allDone.arrive_and_wait();
				}
			}});
	}

	return 0;
}
