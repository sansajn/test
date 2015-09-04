/* Example 11.6. Parsing character by character with boost::spirit::qi::lexeme
http://theboostcpplibraries.com/boost.spirit-parsers#ex.spirit_06 */

#include <string>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		qi::lexeme[ascii::digit >> ascii::digit], ascii::space);
	std::cout << std::boolalpha << match << '\n';
	if (it != s.end())
		std::cout << std::string{it, s.end()} << '\n';
}

