// pristup k dvom intom ako akcie
#include <string>
#include <stdexcept>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost::spirit;
using boost::spirit::qi::int_;

int main(int argc, char * argv[])
{
	std::string s = "11 14";

	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		(int_ >> int_) [
			std::cout << "Matched integer:" << qi::_1 << ", " << qi::_2 << "\n"
		], ascii::space);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};

	std::cout << "done!\n";
	return 0;
} 
