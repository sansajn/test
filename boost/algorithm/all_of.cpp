// all_of family algorithms usage sample
#include <array>
#include <boost/algorithm/cxx11/all_of.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::array;
using boost::algorithm::all_of;
using boost::algorithm::all_of_equal;


TEST_CASE("all_of family algorithms usage sample", 
	"[algorithm][all_of]")
{
	array<int, 3> data = {1, 1, 1};

	REQUIRE(all_of(begin(data), end(data), [](int const & x){
		return x == 1;}));

	REQUIRE(all_of_equal(begin(data), end(data), 1));
}
