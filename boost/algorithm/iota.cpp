// iota algorithm usage sample
#include <array>
#include <boost/algorithm/cxx11/iota.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::array;
using boost::algorithm::iota;

TEST_CASE("copy_if algorithm usage sample", 
	"[algorithm][iota]")
{
	array<int, 5> a;
	iota(begin(a), end(a), 1);

	REQUIRE(a == (array<int, 5>{1, 2, 3, 4, 5}));
}
