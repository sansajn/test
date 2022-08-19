/*! Insert into map with Boost.Assign operator()() and insert() sample. 
Taken from Assign documentation. */
#include <boost/assign/list_inserter.hpp>  // for `insert()`
#include <boost/assert.hpp>
#include <string>
#include <map>

using std::map, std::string;
using boost::assign::insert;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	map<string, int> months;
	insert(months)
		("january",31)
		("february",28)
		("march", 31)
		("april",30);
	BOOST_ASSERT(months.size() == 4);
	BOOST_ASSERT(months["january"] == 31);
	return 0;
}
