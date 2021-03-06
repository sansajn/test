// interruptable thread (jthread) sample
#include <thread>
#include <chrono>
#include <iostream>

using std::jthread, std::stop_token;
using std::cout, std::endl;
using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	jthread th{[]{cout << "hello from jthread!" << endl;}};

	// jthread doesn't need to be joined before exit

	jthread th_interrupt{[](stop_token stok){
		for (int i = 0; i < 10; ++i)
		{
			std::this_thread::sleep_for(200ms);

			if (stok.stop_requested())
			{
				cout << "thread quit requested, quitting!" << endl;
				return;
			}

			cout << "counter: " << i << endl;
		}
		cout << "thread done" << endl;
	}};

	std::this_thread::sleep_for(1s);

	th_interrupt.request_stop();  // signalize thread quit

	cout << "done!\n";
	return 0;
}
