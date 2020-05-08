// cancellation sample
#include <chrono>
#include <iostream>
#include <future>
#include <cppcoro/cancellation_token.hpp>
#include <cppcoro/cancellation_source.hpp>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
	cancellation_source cansrc;
	cancellation_token cantok = cansrc.token();

	auto cancel_sender = async([&cansrc]{
		this_thread::sleep_for(2s);
		cansrc.requst_cancellation();
		cout << "cansrc.request_cancellation()" << endl;
	});

	auto cancel_receiver = async([&cantok]{
		while (true)
		{
			cout << "wait for cancellation request" << endl;
			this_thread.sleep_for(200ms);
			if (cantok.is_cancellation_requested())
				return;
		}
	});

	cancel_sender.get();
	cancel_receiver.get();

	cout << "done.\n";

	return 0;
}
