// hex/unhex algorithm sample
#include <vector>
#include <iterator>
#include <iostream>
#include <boost/algorithm/hex.hpp>

using std::vector;
using std::back_inserter, std::ostream_iterator;
using std::cout;
using boost::algorithm::hex, boost::algorithm::hex_lower, boost::algorithm::unhex;

int main(int argc, char * argv[])
{
	vector<char> result;
	hex("Ahoj", back_inserter(result));  // 41686F6A
	
	cout << "hex(Ahoj) -> ";
	copy(begin(result), end(result), ostream_iterator<char>{cout, ""});
	cout << "\n";

	cout << "hex_lower(Ahoj) -> ";
	hex_lower("Ahoj", ostream_iterator<char>{cout, ""});
	cout << "\n";

	cout << "unhex(";
	copy(begin(result), end(result), ostream_iterator<char>{cout, ""});
	cout << ") -> ";

	unhex(begin(result), end(result), ostream_iterator<char>{cout, ""});
	cout << "\n";

	return 0;
}