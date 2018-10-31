#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/iterator/filter_iterator.hpp>

using std::vector;
using std::copy;
using std::ostream_iterator;
using std::cout;
using boost::filter_iterator;
using boost::make_filter_iterator;

struct is_positive_number
{
	bool operator()(int x) const {return x > 0;}
};

int main(int argc, char * argv[])
{
	vector<int> numbers = {0, -1, 4, 3, 5, 8, -2};

	// direct iterator use
	using positive_numbers = filter_iterator<is_positive_number, vector<int>::const_iterator>;

	copy(positive_numbers{is_positive_number{}, numbers.begin()}, positive_numbers{is_positive_number{}, numbers.end()}, ostream_iterator<int>{cout, " "});
	cout << std::endl;


	// make_filter_iterator
	copy(
		make_filter_iterator<is_positive_number>(numbers.begin(), numbers.end()),
		make_filter_iterator<is_positive_number>(numbers.end(), numbers.end()),
		ostream_iterator<int>(cout, " ")
	);

	cout << std::endl;

	return 0;
}
