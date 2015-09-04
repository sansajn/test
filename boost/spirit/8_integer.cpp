/* Example 11.8. Numeric parsers
http://theboostcpplibraries.com/boost.spirit-parsers#ex.spirit_08 */
#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(), qi::int_, ascii::space);
	std::cout << std::boolalpha << match << '\n';
	if (it != s.end())
		std::cout << std::string{it, s.end()} << '\n';
}