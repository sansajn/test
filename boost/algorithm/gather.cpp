// gather algorithm usage sample
#include <vector>
#include <boost/algorithm/gather.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::vector;
using boost::algorithm::gather;

bool is_even(int x) {return x % 2 == 0;}

TEST_CASE("gather algorthm usage sample",
	"[gather][algorithm]")
{
	vector v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	REQUIRE(gather(begin(v), end(v), begin(v) + 4, is_even) == make_pair(begin(v) + 2, begin(v) + 7));

// 1 3 0 2 4 6 8 5 7 9
//     |---|-----|
//   first |  second
//       pivot
	REQUIRE(v == (vector<int>{1, 3, 0, 2, 4, 6, 8, 5, 7, 9}));
}

