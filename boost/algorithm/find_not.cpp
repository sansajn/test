// find_not algorithm sample
#include <vector>
#include <boost/algorithm/find_not.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using boost::algorithm::find_not;

TEST_CASE("find_not algorithm usage sample",
	"[algorithm][find_not]")
{
	vector<int> v{1, 1, 2};
	REQUIRE(find_not(v, 1) == begin(v) + 2);
}
