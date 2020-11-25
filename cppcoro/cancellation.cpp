// coroutine cancellation sample from [C++20: Powerful Coroutines with cppcoro](https://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro) article

#include <chrono>
#include <iostream>
#include <future>
#include <cppcoro/cancellation_token.hpp>
#include <cppcoro/cancellation_source.hpp>

using std::cout, std::endl;
using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	cppcoro::cancellation_source cansrc;
	cppcoro::cancellation_token cantok = cansrc.token();

	auto cancel_sender = std::async([&cansrc]{
		std::this_thread::sleep_for(2s);
		cansrc.request_cancellation();
		cout << "cansrc.request_cancellation() " << endl;
	});

	auto cancel_receiver = std::async([&cantok]{
		while (true)
		{
			cout << "wait for cancellation request" << endl;
			std::this_thread::sleep_for(200ms);
			if (cantok.is_cancellation_requested())
				return;
		}
	});

	cancel_sender.get(), cancel_receiver.get();

	cout << "done!\n";
	return 0;
}
