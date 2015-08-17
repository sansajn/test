// test rozsahu integer_range (boost/range/irange.hpp)
#include <iterator>
#include <iostream>
#include <boost/range/irange.hpp>

int main(int argc, char * argv[])
{
	boost::integer_range<int> numbers{0, 10};
	std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>{std::cout, ","});
	std::cout << std::endl;
	return 0;
}
