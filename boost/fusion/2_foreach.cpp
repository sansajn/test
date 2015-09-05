/* Example 50.2. Iterating over a tuple with boost::fusion::for_each()
http://theboostcpplibraries.com/boost.fusion#ex.fusion_02 */
#include <string>
#include <iostream>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/algorithm.hpp>

using namespace boost::fusion;

struct print
{
	template <typename T>
	void operator()(const T &t) const
	{
		std::cout << std::boolalpha << t << '\n';
	}
};

int main(int argc, char * argv[])
{
	auto t = make_tuple(10, std::string{"Boost"}, true, 3.14);
	for_each(t, print{});
	return 0;	
}
