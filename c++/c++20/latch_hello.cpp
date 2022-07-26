// latch (`latch.cpp`) sample from *C++20 - The Cpmplete Guide* book,  chapter 12.1.1
#include <iostream>
#include <array>
#include <thread>
#include <latch>
using std::cout, std::array, std::latch, std::jthread;
using namespace std::literals;


void loopOver(char c) {
	for (int j = 0; j < c/2; ++j) {
		cout.put(c).flush();
		std::this_thread::sleep_for(100ms);
	}
}

int main(int argc, char * argv[]) {
	array tags{'.', '?', '8', '+', '-'};
	latch allDone{tags.size()};
	
	jthread t1{[tags, &allDone]{
		for (size_t i = 0; i < size(tags); i += 2) {  // even indexes
			loopOver(tags[i]);
			allDone.count_down(); // signal that the task is done
		}
	}};
	
	jthread t2{[tags, &allDone]{
		for (size_t i = 1; i < size(tags); i += 2) {  // odd indexes
			loopOver(tags[i]);
			allDone.count_down(); // signal that the task is done
		}
	}};

	cout << "\n"
		<< "waiting until all tasks are done\n";

	allDone.wait();  // wait until counter of latch is zero

	cout << "\n"
		<< "all tasks done\n";

	return 0;
}
