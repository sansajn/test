/* Example 11.9. Linking actions with parsers
http://theboostcpplibraries.com/boost.spirit-actions#ex.spirit_09 */
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
		qi::int_[([](int i){ std::cout << i << '\n'; })], ascii::space);
	std::cout << std::boolalpha << match << '\n';
	if (it != s.end())
		std::cout << std::string{it, s.end()} << '\n';
}