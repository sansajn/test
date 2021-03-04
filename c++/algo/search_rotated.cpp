// search in rotated sorted array `[4, 5, 6, 7, 0, 1, 2]` with partition_point and binary_search algorithms
#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>

using std::partition_point, std::binary_search, std::vector, std::cout;

bool search(vector<int> & nums, int target)
{
	auto f = begin(nums);
	auto l = end(nums);
	auto p = partition_point(f, l,
		[x = *f](auto const & e) {return e >= x;});
	return target >= *f 
		? binary_search(f, l, target) : binary_search(p, l, target);
}

int main(int argc, char * argv[])
{
	vector d = {4, 5, 6, 7, 0, 1, 2};
	bool present = search(d, 5);
	cout << "5 is present: " << std::boolalpha << present << "\n";
	return 0;
}
