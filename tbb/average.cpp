#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/task_scheduler_init.h>

using std::cout;
using namespace tbb;

int const GRAIN_SIZE = 10;

struct average 
{
	float * input;
	float * output;
	
	void operator()(blocked_range<int> const & r) const
	{
		for (int i = r.begin(); i != r.end(); ++i)
			output[i] = (input[i-1] + input[i] + input[i+1])*(1/3.0f);
	}
};


int main(int argc, char * argv[])
{
	task_scheduler_init init;
	
	float input[1024];
	for (int i = 0; i < 1024; ++i)
		input[i] = 1.0f * i;
	float output[1024];
	int const n = 1024;
	
	average avg;
	avg.input = input+1;
	avg.output = output;
	parallel_for(blocked_range<int>{0, n-1, GRAIN_SIZE}, avg);
	
	cout << "done!\n";
	return 0;
}
