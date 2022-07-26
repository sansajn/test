// latch based thread synchronization (`latchready.cpp`) sample from *C++20 - The Cpmplete Guide* book,  chapter 12.1.1
#include <iostream>
#include <array>
#include <thread>
#include <latch>
#include <vector>
using std::cout, std::array, std::latch, std::jthread, std::vector;
using namespace std::literals;

int main(int argc, char * argv[]) {
	constexpr ptrdiff_t numThreads = 10;
	
	latch allReady{numThreads};
	
	vector<jthread> threads;
	for (size_t i = 0; i < numThreads; ++i) {
		jthread t{[i, &allReady]{
			std::this_thread::sleep_for(100ms * i);
			
			allReady.arrive_and_wait();  // decrease and wait so that all threads start together from there

			// do some work there ...
			for (int j = 0; j < i+5; ++j) {
				cout.put(static_cast<char>('0'+i)).flush();
				std::this_thread::sleep_for(50ms);
			}
		}};
		threads.push_back(move(t));
	}

	return 0;
}
