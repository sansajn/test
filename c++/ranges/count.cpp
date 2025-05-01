// count algorithm sample
#include <algorithm>
#include <vector>
#include <iostream>
using std::vector, std::cout;

int main(int argc, char * argv[]) {
	vector d = {1,2,3,1,1};
	int count_1 = std::ranges::count(d, 1);  //= 3
	cout << "count_1=" << count_1 << '\n';
	return 0;
}
