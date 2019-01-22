#include <iostream>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

int const GRAIN_SIZE = 10;

using std::cout;
using namespace tbb;

void Foo(float a)
{
	cout << a << " ";
}

class apply_foo  // body object
{
public:
	void operator()(blocked_range<size_t> const & r) const 
	{
		float * a = _a;
		for (size_t i = r.begin(); i != r.end(); ++i)
			Foo(a[i]);
	}
	
	apply_foo(float a[])
		: _a{a}
	{}
	
private:
	float * const _a;
};

int main(int argc, char * argv[])
{
	task_scheduler_init init;
	
	float a[1024];
	size_t n = 1024;
	for (size_t i = 0; i < n; ++i)
		a[i] = 1.0*i;
	
	parallel_for(blocked_range<size_t>{0, n, GRAIN_SIZE}, apply_foo{a});
	
	return 0;
}
