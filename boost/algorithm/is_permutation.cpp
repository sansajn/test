// is_permutation algorithm usage sample
#include <array>
#include <boost/algorithm/cxx11/is_permutation.hpp>

using std::array;
using boost::algorithm::is_permutation;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("is_permutation algorithm usage sample", 
	"[algorithm][is_permutation]")
{
	array<int, 5> a = {1, 2, 3, 4, 5};
	array<int, 5> b = {3, 5, 4, 1, 1};
	array<int, 5> c = {3, 5, 4, 1, 2};
	array<int, 3> d = {1, 2, 3};
	array<int, 6> e = {1, 2, 5, 4, 6, 3};

	REQUIRE_FALSE(is_permutation(a, begin(b)));
	REQUIRE(is_permutation(a, begin(c)));
	REQUIRE_FALSE(is_permutation(a, begin(d)));
	REQUIRE_FALSE(is_permutation(a, begin(e)));
}
