// for_each_n algorithm sample
#include <vector>
#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using std::for_each_n;

TEST_CASE("for_each_n algorithm usage sample", 
	"[algorithm][for_each_n][cxx17]")
{
	vector<int> a{1, 2, 3};
	for_each_n(begin(a), 3, [](int & x){x += 1;});

	REQUIRE(a == (vector<int>{2, 3, 4}));
}
