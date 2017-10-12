// pouzitie std::packaged_task
#include <chrono>
#include <iostream>
#include <future>
#include <thread>

using std::cout;

int my_computation(int x)
{
	for (unsigned i = 0; i < 999999999; ++i)
		x++;
	return x;
}

int main(int argc, char * argv[])
{
	std::packaged_task<int (int)> task1{my_computation};
	std::packaged_task<int (int)> task2{my_computation};

	std::future<int> val1 = task1.get_future();
	std::future<int> val2 = task2.get_future();

	// schedule promises
	std::thread t1{std::move(task1), 0};
	std::thread t2{std::move(task2), 5};

	// print status while we wait
	bool s1 = false, s2 = false;
	do 
	{
		s1 = val1.wait_for(std::chrono::milliseconds{50}) == std::future_status::ready;
		s2 = val2.wait_for(std::chrono::milliseconds{50}) == std::future_status::ready;
		cout << "Value 1 is " << (s1 ? "" : "not ") << "ready" << std::endl;
		cout << "Value 2 is " << (s2 ? "" : "not ") << "ready" << std::endl;
	}
	while (!s1 || !s2);

	t1.join();
	t2.join();

	// final result
	cout << "Value 1: " << val1.get() << std::endl;
	cout << "Value 2: " << val2.get() << std::endl;

	return 0;
}
