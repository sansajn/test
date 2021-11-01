// testing float numbers
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("we can use Approx to test floats", "[floats]") {
	double a = 1.0;
	double b = a + std::numeric_limits<double>::epsilon();
	REQUIRE_FALSE(b == a);
	REQUIRE(b == Approx(a));
	REQUIRE(b == Approx(a).epsilon(0.1));
}
