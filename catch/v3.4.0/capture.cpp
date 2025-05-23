#include <catch2/catch_test_macros.hpp>

TEST_CASE("Testing CAPTURE with simple values") {
	int const a = 5,
		b = 10;

	CAPTURE(a, b);  // capture the a, b variables

	// Performing a test
	int const result = a + b;
	REQUIRE(result == 15);  // OK

	REQUIRE(result == 10);  // this fails with following message (see *with messages:* section)
/*
FAILED:
  REQUIRE( result == 10 )
with expansion:
  15 == 10
with messages:
  a := 5
  b := 10
*/
}