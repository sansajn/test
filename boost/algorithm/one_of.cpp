// one_of family algorithms usage sample
#include <array>
#include <boost/algorithm/cxx11/one_of.hpp>

using std::array;
using boost::algorithm::one_of;
using boost::algorithm::one_of_equal;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

bool is_odd(int i) {return i % 2 == 1;}
bool less_than_10(int i) {return i < 10;}

TEST_CASE("one_of family algorithms usage sample", 
	"[algorithm][one_of]")
{
	array<int, 6> data = {0, 1, 2, 3, 14, 15};
	
	REQUIRE_FALSE(one_of(begin(data), end(data), is_odd));
	REQUIRE_FALSE(one_of(begin(data), end(data), less_than_10));
	REQUIRE(one_of(begin(data) + 3, end(data), less_than_10));
	
	REQUIRE(one_of_equal(data, 3));
}
