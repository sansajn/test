// thread synchronisation with tasks
#include <future>
#include <iostream>
#include <utility>

using namespace std;

void do_the_work()
{
	cout << "processing shared data" << endl;
}

void waiting_for_work(future<void> && f)
{
	cout << "worker: waiting for work" << endl;
	f.wait();
	do_the_work();
	cout << "work done" << endl;
}

void set_data_ready(promise<void> && p)
{
	cout << "sender: data is ready" << endl;
	p.set_value();
}

int main(int argc, char * argv[])
{
	promise<void> send_ready;
	future<void> ready_future = send_ready.get_future();

	thread t1{waiting_for_work, move(ready_future)};
	thread t2{set_data_ready, move(send_ready)};

	t1.join();
	t2.join();

	return 0;
}
