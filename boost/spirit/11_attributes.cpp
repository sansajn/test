/* Example 11.11. Storing an int value in an attribute
http://theboostcpplibraries.com/boost.spirit-attributes#ex.spirit_11 */
#include <string>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	int i;
	if (qi::phrase_parse(it, s.end(), qi::int_, ascii::space, i))
		std::cout << i << '\n';
}
