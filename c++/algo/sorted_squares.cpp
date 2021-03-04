// return sorted squere roots
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

using std::transform, std::sort, std::vector, std::copy, std::ostream_iterator, 
	std::cout;

void sorted_squares(vector<int> & A)
{
	transform(begin(A), end(A), begin(A), [](auto e){return e*e;});
	sort(begin(A), end(A));
}

int main(int argc,char * argv[])
{
	vector d = {1, 4, 2, 6, 7, 3};
	sorted_squares(d);

	copy(begin(d), end(d), ostream_iterator<int>{cout, ", "});
	cout << "\n";

	return 0;
}
