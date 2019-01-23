#include <iostream>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <tbb/task_scheduler_init.h>

using std::cout;
using namespace tbb;

struct sum
{
	float value;
	sum() : value{0} {}
	sum(sum & s, split) {value = 0;}
	
	void operator()(blocked_range<float *> const & r)
	{
		float temp = value;
		for (float * a = r.begin(); a != r.end(); ++a)
			temp += *a;
		value = temp;
	}
	
	void join(sum & y) {value += y.value;}
};

int main(int argc, char * argv[])
{
	task_scheduler_init init;
	
	int const n = 1024*1024;
	float input[n];
	for (int i = 0; i < n; ++i)
		input[i] = i+1.0f;
	
	sum total;
	parallel_reduce(blocked_range<float *>{input, input+n}, total, auto_partitioner{});
	
	cout << "total=" << total.value << "\n";
	cout << "done!\n";
	return 0;
}
