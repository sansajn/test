// array to range using iterator_range sample
#include <iostream>
#include <boost/range/iterator_range.hpp>
using std::cout;
using boost::iterator_range;

int main(int argc, char * argv[])
{
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	auto rn = iterator_range<int *>{data, data+10};

	for (int val : rn)
		cout << val << ", ";
	cout << "\n";

	// note: iterator_range not working with join()
	
	return 0;
}
