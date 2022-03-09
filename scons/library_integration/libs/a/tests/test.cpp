#include <catch.hpp>
#include "a/vec3.hpp"

TEST_CASE("we can create constexpr vec3", "[vec3]") {
	constexpr vec3 u{1, 1, 0},
		v{2, 2, 0},
		k = u + v,
		l = u - v,
		m = u*10.0,
		n = 10*v;

	REQUIRE(u == {1,1,0});
}
