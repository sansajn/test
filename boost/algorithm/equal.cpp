// equal algorithm usage sample
#include <vector>
#include <boost/algorithm/cxx14/equal.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;

TEST_CASE("equal algorithm usage sample", 
	"[algorithm][equal][cxx14]")
{
	vector<int> a{1, 2, 3, 4, 5};
	vector<int> b{1, 2, 3};

	REQUIRE_FALSE(boost::algorithm::equal(begin(a), end(a), begin(b), end(b)));
	REQUIRE(boost::algorithm::equal(begin(a), begin(a) + 3, begin(b), end(b)));
}
