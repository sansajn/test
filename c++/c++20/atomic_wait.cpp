/*! atomic waitable API sample taken from *C++20 The Complete Guide*, 
chapter 12, sample `lib/atomicwait.cpp`

\note It is not guaranteed to get all updates, output can looks this way

```console
$ ./atomic_wait 
=> x changed to 0
WRITER DONE
=> x changed to 42
READER DONE
``` */
#include <iostream>
#include <thread>
#include <atomic>
using std::atomic, std::jthread;
using std::cout;
namespace this_thread = std::this_thread;
using namespace std::literals;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	atomic<int> a_val{0};
	
	// reader
	jthread t_read{[&]{
		int last_x = a_val.load();
		while (last_x >= 0) {
			a_val.wait(last_x);
			cout << "=> x changed to " << last_x << std::endl;
			last_x = a_val.load();
		}
		cout << "READER DONE" << std::endl;
	}};

	// writer
	jthread t_write{[&]{
		for (int new_val : {17, 34, 4, 42, -1}) {
			this_thread::sleep_for(5ns);
			a_val = new_val;
			a_val.notify_all();
		}
		cout << "WRITER DONE" << std::endl;
	}};

	return 0;
}