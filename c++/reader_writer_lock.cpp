// reader-writer lock with std::shared_mutex sample
#include <shared_mutex>
#include <thread>
#include <string_view>
#include <vector>
#include <chrono>
#include <iostream>

using std::shared_mutex, std::shared_lock, std::unique_lock;
using std::thread;
using std::string_view;
using std::vector;
using std::cout, std::endl;
using namespace std::chrono_literals;

shared_mutex content_locker;

void reader(char initial) 
{
	for (int i = 0; i < 10; ++i)
	{
		{
			shared_lock lock{content_locker};
			std::this_thread::sleep_for(2ms);
			cout << initial;
		}

		std::this_thread::sleep_for(10ms);
	}
}

void writer(vector<string_view> const & text) 
{
	for (string_view const & w : text)
	{
		{
			unique_lock lock{content_locker};  // whole world needs to be written without an interruption
			for (auto ch : w)
			{
				std::this_thread::sleep_for(2ms);
				cout << ch;
			}
			cout << " ";
		}

		std::this_thread::sleep_for(5ms);
	}
}

vector<string_view> text{
	"The", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog!"};

int main(int argc, char * argv[])
{
	thread w{writer, text};

	thread r1{reader, '.'},
	 	r2{reader, ','},
	 	r3{reader, ';'},
	 	r4{reader, ':'},
	 	r5{reader, '/'};

	w.join();
	r1.join();
	r2.join();
	r3.join();
	r4.join();
	r5.join();

	cout << "\ndone!\n";
	return 0;
}
