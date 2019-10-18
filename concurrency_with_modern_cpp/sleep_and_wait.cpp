// various waiting strategies
#include <utility>
#include <iostream>
#include <future>
#include <thread>
#include <utility>

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace std;

mutex count_locker;

long double difference(steady_clock::time_point const & a, steady_clock::time_point const & b)
{
	steady_clock::duration diff = b - a;
	return duration<long double, milli>{diff}.count();
}

void producer(promise<int> && prom)
{
	cout << "PRODUCING THE VALUE 42\n" << endl;
	this_thread::sleep_for(5s);
	prom.set_value(42);
}

void consumer(shared_future<int> fut, steady_clock::duration dur)
{
	steady_clock::time_point start = steady_clock::now();
	future_status stat = fut.wait_until(steady_clock::now() + dur);
	if (stat == future_status::ready)
	{
		lock_guard<mutex> lock{count_locker};
		cout << this_thread::get_id() << " ready => Result: " << fut.get() << endl;
	}
	else
	{
		lock_guard<mutex> lock{count_locker};
		cout << this_thread::get_id() << " stopped waiting." << endl;
	}

	steady_clock::time_point end = steady_clock::now();
	lock_guard<mutex> lock{count_locker};
	cout << this_thread::get_id() << " waiting time: " << difference(start, end)
		<< " ms" << endl;
}

void consume_periodically(shared_future<int> fut)
{
	steady_clock::time_point start = steady_clock::now();
	future_status stat;

	do {
		this_thread::sleep_for(700ms);
		stat = fut.wait_for(0s);
		if (stat == future_status::timeout)
		{
			lock_guard<mutex> lock{count_locker};
			cout << "     " << this_thread::get_id()
				<< " still waiting." << endl;
		}
		if (stat == future_status::ready)
		{
			lock_guard<mutex> lock{count_locker};
			cout << "     " << this_thread::get_id()
				<< " waiting done => Result: " << fut.get() << endl;
		}
	}
	while (stat != future_status::ready);

	steady_clock::time_point end = steady_clock::now();
	lock_guard<mutex> lock{count_locker};
	cout << "     " << this_thread::get_id() << " waiting time: "
		<< difference(start, end) << " ms" << endl;
}

void consume_with_back_off(shared_future<int> fut)
{
	steady_clock::time_point start = steady_clock::now();
	future_status stat;
	steady_clock::duration dur = 1ms;

	do {
		this_thread::sleep_for(dur);
		stat = fut.wait_for(0s);
		dur *= 2;
		if (stat == future_status::timeout)
		{
			lock_guard<mutex> lock{count_locker};
			cout << "         " << this_thread::get_id() << " still waiting." << endl;
		}
		if (stat == future_status::ready)
		{
			lock_guard<mutex> lock{count_locker};
			cout << "         " << this_thread::get_id() << " waiting done => Result: "
				<< fut.get() << endl;
		}
	}
	while (stat != future_status::ready);

	steady_clock::time_point end = steady_clock::now();
	lock_guard<mutex> lock{count_locker};
	cout << "     " << this_thread::get_id() << " waiting time: "
		<< difference(start, end) << " ms" << endl;
}

int main(int argc, char * argv[])
{
	promise<int> prom;
	shared_future<int> fut = prom.get_future();
	thread producer_thread{producer, move(prom)};

	thread consumer_thread_1{consumer, fut, 4s};
	thread consumer_thread_2{consumer, fut, 20s};
	thread consumer_thread_3{consume_periodically, fut};
	thread consumer_thread_4{consume_with_back_off, fut};

	consumer_thread_1.join();
	consumer_thread_2.join();
	consumer_thread_3.join();
	consumer_thread_4.join();

	producer_thread.join();

	return 0;
}
