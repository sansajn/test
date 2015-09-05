/* Example 50.3. Filtering a Fusion container with boost::fusion::filter_view
http://theboostcpplibraries.com/boost.fusion#ex.fusion_03 */
#include <string>
#include <iostream>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/view.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/arg.hpp>

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
	using tuple_t = tuple<int, std::string, bool, double>;
	tuple_t t{10, "Boost", true, 3.14};
	filter_view<tuple_t, boost::is_integral<boost::mpl::arg<1>>> v{t};  // mpl::arg<1> je placeholder
	for_each(v, print{});
	return 0;
}
