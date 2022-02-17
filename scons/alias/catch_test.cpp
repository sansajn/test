#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "vec3.hpp"

TEST_CASE("we can add two constexpr vectors") {
	constexpr vec3 v{1,2,3},
		u{4,5,6},
		k=v+u;

	REQUIRE((k == vec3{5,7,9}));
}
