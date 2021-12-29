// possible issue samples
#include <string>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::string;

TEST_CASE("null pointer dereference", "[1][sanitizer]") {
	int * i = nullptr;
	*i = 42;  // null pointer dereference
	REQUIRE(*i == 42);
}

TEST_CASE("out of range access", "[2][sanitizer]") {
	// out of range access
	char const * names[] = {"bob", "fred"};
	string missing = names[3];
	REQUIRE(missing == "<undefined>");
}

// uninitialized variable
void set_b(bool & b, int val) {
	if (val > 0)
		b = false;
}

TEST_CASE("uninitialize variable", "[3][sanitizer]") {
	bool b;
	set_b(b, 0);
	REQUIRE_FALSE(b);  // uninitalized value access
}

TEST_CASE("memory leak", "[4][sanitizer]") {
	int * i = new int{42};
	i = new int{*i + 1};  // memory leak there
	REQUIRE(*i == 43);
}
