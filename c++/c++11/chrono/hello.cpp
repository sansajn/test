// zakladne pouzitie chrono kniznice
#include <chrono>
#include <iostream>

void do_something();

int main(int argc, char * argv[]) 
{
	using clock = std::chrono::high_resolution_clock;
	clock::time_point t = clock::now();
	do_something();
	clock::duration d = clock::now() - t;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << " ms\n";
	return 0;
}

void do_something()
{
	volatile int count;
	for (count = 0; count < std::numeric_limits<int>::max(); ++count)
		;
}
