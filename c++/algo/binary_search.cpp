// searching in sorted array with binary_search algorithm sample
#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>

using std::binary_search, std::vector, std::cout;

int main(int argc, char * argv[])
{
	vector d = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	bool present = binary_search(begin(d), end(d), 5);
	cout << "5 is present: " << std::boolalpha << present << "\n";
	return 0;
}
