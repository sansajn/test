// parsovanie obsahu v "" nieco ako "\.*"
#include <algorithm>
#include <string>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;
using qi::char_;

void print(std::vector<char> const & s)
{
	std::copy(s.begin(), s.end(), std::ostream_iterator<char>{std::cout, ""});
	std::cout << "\n";
}

void inplace_parser()
{
	std::string s = R"("arm.L")";
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		'"' >> (*(char_ - '"'))[print] >> '"',
		ascii::space);
	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse '"} + std::string{it, s.end()} + "' expression"};
	std::cout << "inplce_parser(): done\n";
}

void print_as_string(std::string const & s)
{
	std::cout << s << "\n";
}

void as_rule()
{
	std::string s = R"("   arm.L")";

	qi::rule<std::string::iterator, std::string(), ascii::space_type> qstring =
		qi::lexeme['"' >> *(char_ - '"') >> '"'];

	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(), qstring[print_as_string], ascii::space);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse '"} + std::string{it, s.end()} + "' expression"};

	std::cout << "as_rule(): done\n";
}

int main(int argc, char * argv[])
{
	inplace_parser();
	as_rule();
	std::cout << "done!\n";
	return 0;
}
