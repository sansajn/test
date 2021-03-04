/*! nth_element algorithm sample: get K closest points from (0,0) */
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

using std::vector, std::nth_element, std::ostream_iterator, std::cout;

int main(int argc, char * argv[])
{
	vector<vector<int>> points = {{10,2}, {2,20}, {10,15}, {0,0}, {30,0}, {1,1}, {3,14}};

	int K = 3;

	nth_element(begin(points), begin(points) + K, end(points), 
		[](auto const & a, auto const & b){
			return (a[0]*a[0] + a[1]*a[1]) < (b[0]*b[0] + b[1]*b[1]);
		});
	// in case of K sorted elementswe can use partial_sort() algorithm there

	for (auto it = begin(points); it != begin(points)+K; ++it)
		cout << "(" << (*it)[0] << ", " << (*it)[1] << "), ";
	cout << "\n";

	return 0;
}
