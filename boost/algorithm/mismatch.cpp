// mismatch algorithm usage sample
#include <vector>
#include <boost/algorithm/cxx14/mismatch.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;

TEST_CASE("mismatch algorithm usage sample", 
	"[algorithm][mismatch][cxx14]")
{
	vector<int> a{1, 2, 3, 4, 5};
	vector<int> b{1, 2, 3};

	REQUIRE(mismatch(begin(a), end(a), begin(b), end(b)) == make_pair(begin(a) + 3, end(b)));
}
