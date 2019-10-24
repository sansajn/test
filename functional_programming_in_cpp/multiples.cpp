#include <functional>
#include <numeric>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	vector numbers{1, 2, 3, 4};
	int product = accumulate(numbers.begin(), numbers.end(), 1, multiplies{});
	cout << "product=" << product << "\n";  // 24 expected

	return 0;
}
