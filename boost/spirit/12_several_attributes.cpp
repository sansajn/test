#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	std::vector<int> v;
	if (qi::phrase_parse(it, s.end(), qi::int_ % ',', ascii::space, v))
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>{std::cout, ";"});
	std::cout << "\n";
}
