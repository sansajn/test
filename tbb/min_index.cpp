#include <iostream>
#include <cfloat>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>

using std::cout;
using namespace tbb;

int const GRAIN_SIZE = 100;

/* long serial_min_index_foo(float const[], size_t n)
{
	float value_of_min = FLT_MAX;
	long index_of_min = -1;
	for (size_t i = 0; i < n; ++i)
	{
		float value = Foo(a[i]);
		if (value < value_of_min)
		{
			value_of_min = value;
			index_of_min = i;
		}
	}
	return index_of_min;
} */
class min_index_foo
{
public:
	float value_of_min;
	long index_of_min;
	
	void operator()(blocked_range<size_t> const & r)
	{
		float const * a = _a;
		for (size_t i = r.begin(); i != r.end(); ++i)
		{
			float value = a[i];
			if (value < value_of_min)
			{
				value_of_min = value;
				index_of_min = i;
			}
		}
	}
	
	min_index_foo(min_index_foo & x, split)
		: value_of_min{FLT_MAX}
		, index_of_min{-1}
		, _a{x._a}
	{}
	
	void join(min_index_foo const & y)
	{
		if (y.value_of_min < value_of_min)
		{
			value_of_min = y.value_of_min;
			index_of_min = y.index_of_min;
		}
	}
	
	min_index_foo(float const a[])
		: value_of_min{FLT_MAX}
		, index_of_min{-1}
		, _a{a}
	{}
	
private:
	float const * _a;
};

int main(int argc, char * argv[])
{
	task_scheduler_init init;
	
	size_t const n = 1024*1024;
	float input[n];
	for (size_t i = 0; i < n; ++i)
		input[i] = i+1.0f;
	input[1234] = -1.0f;
	
	min_index_foo mif{input};
	parallel_reduce(blocked_range<size_t>{0, n, GRAIN_SIZE}, mif);
	
	cout << "index_of_min=" << mif.index_of_min << "\n";
	cout << "done!\n";
	
	return 0;
}
