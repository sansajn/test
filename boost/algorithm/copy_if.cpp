// copy_if algorithm usage sample
#include <vector>
#include <boost/algorithm/cxx11/copy_if.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using boost::algorithm::copy_if,
	boost::algorithm::copy_while,
	boost::algorithm::copy_until;

bool less_than_10(int i) {return i < 10;}

TEST_CASE("copy_if algorithm usage sample", 
	"[algorithm][copy][copy_if]")
{
	vector data = {0, 3, 2, 1, 14, 15};
	
	vector<int> result;
	copy_if(data, back_inserter(result), less_than_10);

	REQUIRE(result == (vector{0, 3, 2, 1}));
}

TEST_CASE("copy_while algorithm usage sample", 
	"[algorithm][copy][copy_while]")
{
	vector data = {0, 3, 14, 2, 1, 15};
	
	vector<int> result;
	copy_while(data, back_inserter(result), less_than_10);

	REQUIRE(result == (vector{0, 3}));
}

TEST_CASE("copy_until algorithm usage sample", 
	"[algorithm][copy][copy_until]")
{
	vector data = {14, 0, 3, 2, 1, 15};
	
	vector<int> result;
	copy_until(data, back_inserter(result), less_than_10);

	REQUIRE(result == (vector{14}));
}
