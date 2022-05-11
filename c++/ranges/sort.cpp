// std::ranges::sort() algorithm in action
#include <algorithm>
#include <array>
#include <iostream>
#include <cassert>
using std::ranges::sort, std::array, std::cout;

int main(int argc, char * argv[]) {
	array<int, 6> a{6, 2, 3, 4, 5, 1};
	sort(a);

	assert((a == array<int, 6>{1, 2, 3, 4, 5, 6}));

	// show result
	cout << "a: [";
	for (int n: a) 
		cout << n << ",";
	cout << "]\n";

	cout << "done\n";
	return 0;
}
