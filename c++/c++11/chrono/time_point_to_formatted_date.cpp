// time_point to string formatted date
#include <chrono>
#include <iostream>

using std::chrono::high_resolution_clock;

int main(int argc, char * argv[])
{
	auto now = high_resolution_clock::now();
	std::time_t t = high_resolution_clock::to_time_t(now);
	std::cout << std::ctime(&t) << std::endl;

	return 0;
}
