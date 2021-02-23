// promise/future pair usage sample
#include <future>
#include <thread>
#include <utility>
#include <iostream>
using std::promise, std::future;
using std::thread;
using std::move;
using std::cout;

void meaning_of_life(promise<int> && answer)
{
	answer.set_value(42);
}

int main(int argc, char * argv[])
{
	promise<int> answer_promise;
	future<int> answer = answer_promise.get_future();

	thread th{meaning_of_life, move(answer_promise)};
	
	cout << "The answer is " << answer.get() << "\n";
	
	th.join();  // wait for thread

	cout << "done!\n";
	return 0;
}
