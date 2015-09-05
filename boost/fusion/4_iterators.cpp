/* Example 50.4. Accessing elements in Fusion containers with iterators
http://theboostcpplibraries.com/boost.fusion#ex.fusion_04 */
#include <string>
#include <iostream>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/mpl/int.hpp>

using namespace boost::fusion;

int main()
{
	typedef tuple<int, std::string, bool, double> tuple_type;
	tuple_type t{10, "Boost", true, 3.14};
	auto it = begin(t);
	std::cout << *it << '\n';
	auto it2 = advance<boost::mpl::int_<2>>(it);  // inkrementacia iteratoru o 2
	std::cout << std::boolalpha << *it2 << '\n';
	return 0;
}
