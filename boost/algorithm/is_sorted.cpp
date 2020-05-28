// is_sorted family algorithms usage sample
#include <algorithm>
#include <array>
#include <boost/algorithm/cxx11/is_sorted.hpp>

using std::sort;
using std::array;
using boost::algorithm::is_sorted;
using boost::algorithm::is_sorted_until;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>


TEST_CASE("is_sorted family algorithms usage sample", 
	"[algorithm][is_sorted]")
{
	array<int, 6> data = {0, 3, 2, 1, 14, 15};
	
	REQUIRE_FALSE(is_sorted(data));

	sort(begin(data), end(data));

	REQUIRE(is_sorted(begin(data), end(data)));

	array<int, 5> data2 = {0, 1, 5, 3, 6};
	REQUIRE(is_sorted_until(data2) == begin(data2) + 3);
}
