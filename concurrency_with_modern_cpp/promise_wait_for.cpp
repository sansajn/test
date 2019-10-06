// waiting for promise

#include <future>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::chrono_literals;
using namespace std;

void get_answer(promise<int> && p)
{
	this_thread::sleep_for(3s);
	p.set_value(42);
}

int main(int argc, char * argv[])
{
	promise<int> answer_promise;
	future<int> answer_future = answer_promise.get_future();

	thread prod_thread{get_answer, move(answer_promise)};

	future_status status;
	do	{
		status = answer_future.wait_for(0.2s);
		cout << "... doing something else" << endl;
	}
	while (status != future_status::ready);

	cout << "\n"
		<< "the answer: " << answer_future.get() << endl;

	prod_thread.join();

	return 0;
}
