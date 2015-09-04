/* Example 11.1. Using boost::spirit::qi::parse() 
http://theboostcpplibraries.com/boost.spirit-api */
#include <string>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	bool match = qi::parse(it, s.end(), ascii::digit);
	std::cout << std::boolalpha << match << '\n';
	if (it != s.end())
		std::cout << std::string{it, s.end()} << '\n';
}
