// none_of family algorithms usage sample
#include <array>
#include <boost/algorithm/cxx11/none_of.hpp>

using std::array;
using boost::algorithm::none_of;
using boost::algorithm::none_of_equal;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("none_of family algorithms usage sample", 
	"[algorithm][none_of]")
{
	array<int, 3> data = {1, 2, 3};
	
	REQUIRE(none_of(begin(data), end(data), [](int const & x){
		return x == 4;}));

	REQUIRE(none_of_equal(begin(data), end(data), 4));
}
