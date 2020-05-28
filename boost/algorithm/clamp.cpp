// clamp algorithm sample
#include <vector>
#include <boost/algorithm/clamp.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using boost::algorithm::clamp;
using boost::algorithm::clamp_range;

TEST_CASE("clamp algorithm usage sample",
	"[algorithm][clamp]")
{
	int val = 5;
	REQUIRE(clamp(val, 1, 3) == 3);
}

TEST_CASE("clamp_range algorithm usage sample",
	"[algorithm][clamp_range]")
{
	vector<int> v{1, 5, 4, 2};
	clamp_range(begin(v), end(v), begin(v), 1, 3);
	REQUIRE(v == (vector<int>{1, 3, 3, 2}));
}
