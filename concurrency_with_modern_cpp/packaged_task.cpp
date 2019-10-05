// concurrency with std::packaged_task

#include <utility>
#include <future>
#include <iostream>
#include <thread>
#include <deque>

using namespace std;

struct sum_up
{
	int operator()(int beg, int end)
	{
		long long sum = 0;
		for (int i = beg; i < end; ++i)
			sum += i;
		return static_cast<int>(sum);
	}
};

int main(int argc, char * argv[])
{
	sum_up sum_up1, sum_up2, sum_up3, sum_up4;

	// wrap the tasks
	packaged_task<int (int, int)> sum_task1{sum_up1}, sum_task2{sum_up2},
		sum_task3{sum_up3}, sum_task4{sum_up4};

	// create futures
	future<int> sum_result1 = sum_task1.get_future();
	future<int> sum_result2 = sum_task2.get_future();
	future<int> sum_result3 = sum_task3.get_future();
	future<int> sum_result4 = sum_task4.get_future();

	// collect tasks
	deque<packaged_task<int (int, int)>> all_tasks;
	all_tasks.push_back(move(sum_task1));
	all_tasks.push_back(move(sum_task2));
	all_tasks.push_back(move(sum_task3));
	all_tasks.push_back(move(sum_task4));

	int begin = 1;
	int increment = 2500;
	int end = begin + increment;

	// perform each calculation in a separate thread
	while (not all_tasks.empty())
	{
		packaged_task<int (int, int)> task = move(all_tasks.front());
		all_tasks.pop_front();
		thread t{move(task), begin, end};
		begin = end;
		end += increment;
		t.detach();
	}

	// pick up the results
	long long sum = sum_result1.get() + sum_result2.get() + sum_result3.get() +
		sum_result4.get();

	cout << "sum of 0 .. 10000 = " << sum << endl;

	return 0;
}
