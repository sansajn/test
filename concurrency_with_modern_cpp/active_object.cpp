// active object pattern implementation
#include <algorithm>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

class is_prime
{
public:
	pair<bool, int> operator()(int i)
	{
		for (int j = 2; j*j <= i; ++j)
		{
			if (i % j == 0)
				return make_pair(false, i);
		}
		return make_pair(true, i);
	}
};

class active_object
{
public:
	using task_pack = pair<packaged_task<pair<bool, int>(int)>, int>;  //!< \note only movable

	future<pair<bool, int>> enqueue_task(int i)
	{
		is_prime prime;
		packaged_task<pair<bool, int>(int)> job{prime};
		auto prime_future = job.get_future();
		task_pack pack = make_pair(move(job), i);

		{
			lock_guard<mutex> lock{_activation_list_locker};
			_activation_list.push_back(move(pack));
		}

		return prime_future;
	}

	void run()
	{
		thread servant([this]{
			while (!is_empty())
			{
				task_pack task = dequeue_task();
				task.first(task.second);
			}
		});
		servant.join();
	}

private:
	task_pack dequeue_task()
	{
		lock_guard<mutex> lock{_activation_list_locker};
		task_pack task = move(_activation_list.front());
		_activation_list.pop_front();
		return task;
	}

	bool is_empty() const
	{
		lock_guard<mutex> lock{_activation_list_locker};
		return _activation_list.empty();
	}

	deque<task_pack> _activation_list;
	mutable mutex _activation_list_locker;
};

vector<int> get_rand_numbers(int count)
{
	random_device seed;
	mt19937 engine{seed()};
	uniform_int_distribution<> dist{1000000, 1000000000};
	vector<int> numbers;
	for (int i = 0; i < count; ++i)
		numbers.push_back(dist(engine));
	return numbers;
}

future<vector<future<pair<bool, int>>>> get_futures(active_object & obj,
	int prime_count)
{
	return async([&obj, prime_count]{
		vector<future<pair<bool, int>>> futures;
		vector<int> rand_numbers = get_rand_numbers(prime_count);
		for (int rnum : rand_numbers)
			futures.push_back(obj.enqueue_task(rnum));
		return futures;
	});
}

int main(int argc, char * argv[])
{
	active_object obj;

	future<vector<future<pair<bool, int>>>> client1 = get_futures(obj, 100),
		client2 = get_futures(obj, 110),
		client3 = get_futures(obj, 115),
		client4 = get_futures(obj, 80),
		client5 = get_futures(obj, 92);

	auto futures1 = client1.get();
	auto futures2 = client2.get();
	auto futures3 = client3.get();
	auto futures4 = client4.get();
	auto futures5 = client5.get();

	// put all futures together
	auto futures = move(futures1);
	futures.insert(futures.end(), make_move_iterator(futures2.begin()),
		make_move_iterator(futures2.end()));

	futures.insert(futures.end(), make_move_iterator(futures3.begin()),
		make_move_iterator(futures3.end()));

	futures.insert(futures.end(), make_move_iterator(futures4.begin()),
		make_move_iterator(futures4.end()));

	futures.insert(futures.end(), make_move_iterator(futures5.begin()),
		make_move_iterator(futures5.end()));

	// run the promisses
	obj.run();

	// get the results from the futures
	vector<pair<bool, int>> results;
	results.reserve(futures.size());
	for (auto & fut : futures)
		results.push_back(fut.get());

	sort(results.begin(), results.end());

	// separate the primes from the non-primes
	auto prime_it = find_if(results.begin(), results.end(),
		[](pair<bool, int> pa){return pa.first == true;});

	cout << "number primes: " << distance(prime_it, results.end()) << "\n"
		<< "primes:\n";

	for (auto it = prime_it; it != results.end(); ++it)
		cout << it->second << " ";

	cout << "\n\n";

	cout << "number no primes: " << distance(results.begin(), prime_it) << "\n"
		<< "no primes:\n";
	for (auto it = results.begin(); it != prime_it; ++it)
		cout << it->second << " ";

	cout << endl;

	return 0;
}
