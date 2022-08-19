// Insert values at the end of vector with Boost.Assign operator+=() overload.
#include <boost/assign/std/vector.hpp>
#include <boost/assert.hpp>
using std::vector;
using namespace boost::assign;  // brings `operator+=()` into scope

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	vector<int> values;
	values += 1,2,3,4,5,6,7,8,9;  // insert values at the end of the container
	BOOST_ASSERT(values.size() == 9);
	BOOST_ASSERT(values[0] == 1);
	BOOST_ASSERT(values[8] == 9);
	return 0;
}
