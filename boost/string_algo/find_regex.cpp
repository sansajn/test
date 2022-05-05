// find_regex() sample
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <string>
#include <iostream>

using std::string, std::cout;
using namespace boost::algorithm;

int main() {
	string s = "Boost C++ Libraries";
	boost::iterator_range<string::iterator> r = find_regex(s, 
		boost::regex{"\\w\\+\\+"});
	cout << r << '\n';
	return 0;
}
