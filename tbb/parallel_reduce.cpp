#include <iostream>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

using std::cout;
using namespace tbb;

int const GRAIN_SIZE = 100;

float Foo(float a)
{
	return a;
}

/* float serial_sum_foo(float a[], size_t n)
{
	float sum = 0;
	for (size_t i = 0; i < n; ++i)
		sum += Foo(a[i]);
	return sum;
} */
class sum_foo 
{
public:
	float sum;
	
	void operator()(blocked_range<size_t> const & r)
	{
		float * a = _a;
		for (size_t i = r.begin(); i != r.end(); ++i)
			sum += Foo(a[i]);
	}
	
	sum_foo(float a[]) : sum{0}, _a{a} {}
	sum_foo(sum_foo & x, split) : sum{0}, _a{x._a} {}
	void join(sum_foo const & y) {sum += y.sum;}
	
private:
	float * _a;
};


int main(int argc, char * argv[])
{
	task_scheduler_init init;
	
	int const n = 1024*1024;
	float input[n];
	for (int i = 0; i < n; ++i)
		input[i] = i + 1;
	
	sum_foo sf{input};
	parallel_reduce(blocked_range<size_t>{0, n, GRAIN_SIZE}, sf);
	cout << "sum=" << sf.sum << "\n";
	cout << "done!\n";
	return 0;
}
