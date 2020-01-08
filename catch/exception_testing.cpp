#define CATCH_CONFIG_MAIN
#include <catch.hpp>

void foo()
{
	throw std::runtime_error{"something bad happend"};
}

void goo() {}


TEST_CASE("we can also test if exception was thrown", "[tag]")
{
	REQUIRE_THROWS(foo());
	REQUIRE_NOTHROW(goo());
}
