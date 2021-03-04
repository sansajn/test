// find element that is repeated 2 times
#include <algorithm>
#include <vector>
#include <iostream>

using std::transform, std::sort, std::adjacent_find, std::vector, std::cout;

int repeated_2_times(vector<int>& A)
{
	sort(begin(A), end(A));
	return *adjacent_find(begin(A), end(A));
}

int main(int argc, char * argv[])
{
	vector d = {0, 1, 2, 3, 4, 5, 6, 7, 8, 5, 9};
	cout << repeated_2_times(d) << "\n";
	return 0;
}
