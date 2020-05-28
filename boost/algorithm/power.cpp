
// power algorithm sample
#include <boost/algorithm/algorithm.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using boost::algorithm::power;

TEST_CASE("power algorithm usage sample"
	"[algorithm][power]")
{
	REQUIRE(power(5, 2) == 25);
}
