// map insert with previous value detection sample
#include <map>
#include <catch.hpp>
using std::map, std::make_pair;

TEST_CASE("map insertion with previous value detection", 
	"[std][map][insert]")
{
	map<bool, int> m;

	// insert brand new value
	auto result = m.insert(make_pair(2, false));
	REQUIRE(result.second);

	// update value
	result = m.insert(make_pair(2, true));
	REQUIRE_FALSE(result.second);
	REQUIRE_FALSE(result.first->second);
	result.first->second = true;
	REQUIRE(m[2]);
}
