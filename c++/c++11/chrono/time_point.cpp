#include <chrono>
#include <iostream>
#include <cassert>

using namespace std::chrono;

// time_point aritmetika
int main(int argc, char * argv[])
{
	using clock = high_resolution_clock;

	// time point infuture
	clock::time_point t = clock::now();
	clock::time_point t_future = t + milliseconds(100);

	assert(t < t_future);

	clock::duration d = t_future - t;

	std::cout << "t:" << duration_cast<milliseconds>(t.time_since_epoch()).count() << "\n";
	std::cout << "t_future:" << duration_cast<milliseconds>(t_future.time_since_epoch()).count() << "\n";
	std::cout << "t_future-t:" << duration_cast<milliseconds>(d).count() << " ms\n";

	return 0;
}