// equal function for {1,2,3} inline testing
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch.hpp>
using std::equal;
using std::transform;
using std::back_inserter;
using std::initializer_list;
using std::vector;

vector<int> times7(vector<int> const & numbers)
{
	vector<int> result;
	transform(begin(numbers), end(numbers), back_inserter(result),
		[](int const & n){return n*7;});
	return result;
}

template <typename Range1, typename Range2>
bool equal(Range1 const & a, Range2 const & b)
{
	return equal(begin(a), end(a), begin(b), end(b));
}

template <typename Range1, typename Value2>
bool equal(Range1 const & a, initializer_list<Value2> const & b)
{
	return equal(begin(a), end(a), begin(b), end(b));
}

template <typename Value1, typename Range2>
bool equal(initializer_list<Value1> const & a, Range2 const & b)
{
	return equal(begin(a), end(a), begin(b), end(b));
}

template <typename Value1, typename Value2>
bool equal(initializer_list<Value1> const & a, initializer_list<Value2> const & b)
{
	return equal(begin(a), end(a), begin(b), end(b));
}

TEST_CASE("basic test for times7", "[test]")
{
	auto const expected = {7, 14, 21, 28, 35};
	auto const inputs = vector{1, 2, 3, 4, 5};
	auto const result = times7(inputs);

	REQUIRE(equal(begin(expected), end(expected), begin(result), end(result)));
	REQUIRE(equal(expected, result));
	REQUIRE(equal(times7(inputs), {7, 14, 21, 28, 35}));
	REQUIRE(equal({7, 14, 21, 28, 35}, times7(inputs)));
	REQUIRE(equal({1, 2, 3}, {1, 2, 3}));
}
