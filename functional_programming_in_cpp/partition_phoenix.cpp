#include <algorithm>
#include <vector>
#include <iostream>
#include <boost/phoenix.hpp>

using namespace std;
using namespace boost::phoenix::arg_names;

int main(int argc, char * argv[])
{
	vector nums{21, 5, 62, 42, 53};

	partition(begin(nums), end(nums), arg1 <= 42);

	// numbers now contain [21, 5, 42,   62, 53]
	//                        <= 42       > 42

	copy(begin(nums), end(nums), ostream_iterator<int>(cout, ", "));
	cout << "\n";

	return 0;
}
