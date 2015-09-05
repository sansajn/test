/* Example 50.1. Processing Fusion tuples 
http://theboostcpplibraries.com/boost.fusion#ex.fusion_01 */
#include <string>
#include <iostream>
#include <boost/fusion/tuple.hpp>

using namespace boost::fusion;

int main(int argc, char * argv[])
{
	using tuple_t = tuple<int, std::string, bool, double>;
	tuple_t t{10, "Boost", true, 3.14};
	std::cout << "{" << get<0>(t) << ", " << get<1>(t) << ", "
		<< std::boolalpha << get<2>(t) << ", " << get<3>(t) << "}\n";
	return 0;
}
