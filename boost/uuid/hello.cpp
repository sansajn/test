// uuid generator sample
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <cassert>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using std::copy, std::begin, std::end;
using std::ostream_iterator;
using std::cout, std::endl;
using std::string, std::to_string;
using boost::uuids::uuid, boost::uuids::random_generator, boost::lexical_cast;

int main(int argc, char * argv[]) {
	// generate random number based uuids
	uuid id1 = random_generator()(),
		id2 = random_generator()();

	assert(id1 != id2);

	copy(begin(id1), end(id1), ostream_iterator<int>{cout, ","});
	cout << endl;

	size_t size = uuid::static_size();
	cout << "uuid-size=" << size << endl;

	// lexical_cast conversion (hhhhhhhh-hhhh-hhhh-hhhh-hhhhhhhhhhhh)
	string s = lexical_cast<string>(id1);
	cout << s << endl;

	// to_string conversion
	s = to_string(id2);
	cout << s << endl;

	return 0;
}