// sample from c++ reference
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include <execution>

using namespace std;

int main(int argc, char * argv[])
{
	vector data{3, 1, 4, 1, 5, 9, 2, 6};

	cout << "exclusive sum: ";
	exclusive_scan(execution::par, data.begin(), data.end(),
		ostream_iterator<int>{cout, " "}, 0);

	cout << "\ninclusive sum: ";
	inclusive_scan(execution::par, data.begin(), data.end(),
		ostream_iterator<int>{cout, " "});

	cout << "\n\nexclusive product: ";
	exclusive_scan(execution::par, data.begin(), data.end(),
		ostream_iterator<int>{cout, " "}, 1, multiplies<>{});

	cout << "\ninclusive product: ";
	inclusive_scan(execution::par, data.begin(), data.end(),
		ostream_iterator<int>{cout, " "}, multiplies<>{});

	cout << endl;

	return 0;
}
