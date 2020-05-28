// apply_permutation algorithm sample

#include <boost/algorithm/apply_permutation.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using boost::algorithm::apply_permutation;

TEST_CASE("apply_permutation algorithm sample", 
	"[apply_permutation][algorithm]")
{
	vector data = {1, 2, 3, 4, 5},
		order = {4, 3, 2, 1, 0};

	apply_permutation(data, order);
	REQUIRE(data == (vector{5, 4, 3, 2, 1}));
}
