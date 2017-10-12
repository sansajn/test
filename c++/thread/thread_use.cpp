// spôsoby vytvárania threadov v pomocou std::thread
#include <thread>
#include <iostream>

using std::thread;
using std::cout;

void do_some_work()
{
	cout << "do_some_work()" << std::endl;
}

struct background_task
{
	void operator()() const
	{
		cout << "background_task::operator()" << std::endl;
	}
};

void do_some_work_with_args(int i, double d)
{
	cout << "do_some_work_with_args(i:" << i << ", d:" << d << ")" << std::endl;
}

struct background_task_with_func
{
	void run()
	{
		cout << "background_task_with_func::run()" << std::endl;
	}
};

int main(int argc, char * argv[])
{
	thread t1{do_some_work};

	background_task f;
	thread t2{f};

	thread t3{background_task{}};

	thread t4{do_some_work_with_args, 3, 1.14};

	background_task_with_func f2;
	thread t5{&background_task_with_func::run, &f2};

	thread t6{&background_task_with_func::run, background_task_with_func{}};

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	return 0;
} 
