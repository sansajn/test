// is_partitioned and partition_point algorithm usage sample
#include <array>
#include <boost/algorithm/cxx11/is_partitioned.hpp>
#include <boost/algorithm/cxx11/partition_point.hpp>
#include <boost/algorithm/is_partitioned_until.hpp>

using std::array;
using boost::algorithm::is_partitioned, 
	boost::algorithm::partition_point,
	boost::algorithm::is_partitioned_until;

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

bool is_odd(int i) {return i % 2 == 1;}
bool less_than_10(int i) {return i < 10;}

TEST_CASE("is_partitioned and partition_point algorithm usage sample", 
	"[algorithm][is_partitioned][partition_point]")
{
	array<int, 6> data = {0, 3, 2, 1, 14, 15};
	
	REQUIRE_FALSE(is_partitioned(data, is_odd));
	REQUIRE(is_partitioned(data, less_than_10));
	REQUIRE(is_partitioned(begin(data), begin(data), is_odd));  // empty range

	REQUIRE(partition_point(data, less_than_10) == begin(data) + 4);

	REQUIRE(is_partitioned_until(data, is_odd) == begin(data) + 1);
}
