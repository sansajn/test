#include <iostream>
#include <cmath>
#include <tbb/parallel_sort.h>

using std::cout;
using namespace tbb;

int main(int argc, char * argv[])
{
	int const N = 100000;
	float a[N];
	float b[N];

	for (int i = 0; i < N; ++i)
	{
		a[i] = sin((double)i);
		b[i] = cos((double)i);
	}

	parallel_sort(a, a + N);
	parallel_sort(b, b + N, std::greater<float>{});

	cout << "done!\n";

	return 0;
}
