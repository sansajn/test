// packaged_task<F> usage sample
#include <future>
#include <thread>
#include <iostream>
using std::packaged_task, std::future;
using std::thread;
using std::cout;

int main(int argc, char * argv[])
{
	packaged_task<int()> meaning_of_life{[](){return 42;}};

	future<int> answer = meaning_of_life.get_future();

	thread th{move(meaning_of_life)};

	cout << "The answer is " << answer.get() << "\n";

	th.join();  // wait for thread

	cout << "done!\n";
	return 0;
}
