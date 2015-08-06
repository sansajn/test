#include <iostream>
using std::cout;
#include <vector>
using std::vector;
#include <algorithm>
using std::sort;
#include <cstdlib>
#include <ctime>


template <class RandomAccessIterator, typename T>
RandomAccessIterator LowerBound(RandomAccessIterator first, 
	RandomAccessIterator last, T const & val)
{
	int len = last - first;
	while (len > 0)
	{
		int half = len/2;
		RandomAccessIterator middle = first + half;
		if (*middle < val)
		{
			first = middle;
			++first;
			len = len-half-1;
		}
		else
			len = half;
	}
	return first;
}


int main()
{
	srand(time(0));
	vector<int> d(1001);
	for (int i = 0; i < 1001; ++i)
		d[i] = rand();
	sort(d.begin(), d.end());
	
	auto it = LowerBound(d.begin(), d.end(), d[752]);
	if (it == d.begin()+752)
		cout << "passed";
	else
		cout << "failed";
	return 0;
}
