/* Example 11.7. Boost.Spirit rules similar to regular expressions
http://theboostcpplibraries.com/boost.spirit-parsers#ex.spirit_07 */
#include <string>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(), +ascii::digit, ascii::space);
	std::cout << std::boolalpha << match << '\n';
	if (it != s.end())
		std::cout << std::string{it, s.end()} << '\n';
}