// section sample
#include <iostream>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::cout;

/* we expect following output

AA
AB

*/
TEST_CASE("sample test case")
{
	cout << "A";  // setup

	SECTION("test case section 1")
	{
		cout << "A";
	}

	SECTION("test case section 2")
	{
		cout << "B";
	}

	cout << "\n";  // teardown
}
