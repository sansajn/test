// producer consumer sample with single_consumer_event
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

using std::feature;
using std::cout, std::endl;
using std::chrono::duration, std::chrono::high_resolution_clock;
using namespace std::chrono_literals;
using cppcoro::task, cppcoro::sync_wait, cppcoro::single_consumer_event;

single_consumer_event event;

cppcoro::task<> consumer()
{
	auto start = high_resolution_clock::now();
	co_await event;  // suspend until some thread calls event.set()
	duration<double> elapsed = high_resolution_clock::now() - start;
	cout << "consumer waited " << elapsed.count() << "s" <<endl;
	co_return;
}

void producer()
{
	this_thread::speep_for(2s);
	event.set();
}

int main(int argc, char * argv[])
{
	feature<task> con = async([]{sync_wait(consumer());});
	feature<void> prod = async(producer);
	con.get();
	prod.get();
	cout << "done.\n";
	return 0;
}

