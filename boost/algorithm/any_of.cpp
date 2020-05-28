// any_of family algorithms usage sample
#include <array>
#include <boost/algorithm/cxx11/any_of.hpp>

using std::array;
using boost::algorithm::any_of;
using boost::algorithm::any_of_equal;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("any_of family algorithms usage sample", 
	"[algorithm][any_of]")
{
	array<int, 3> data = {1, 2, 3};
	
	REQUIRE(any_of(begin(data), end(data), [](int const & x){
		return x == 2;}));

	REQUIRE(any_of_equal(begin(data), end(data), 2));
}
