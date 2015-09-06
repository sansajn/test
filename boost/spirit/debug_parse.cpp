// parsuje dvojice cisel oddelenych novym riadkom alebo znakom ';'
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fusion = boost::fusion;

using qi::int_;
using qi::double_;

int main(int argc, char * argv[])
{
	std::string s =
R"(12 1.4;
63 13.2
2423 56.4 ; 5 8.1
)";

	std::vector<std::pair<int, double>> pairs;

	qi::rule<std::string::iterator, std::pair<int, double>(), ascii::space_type> number_pair = int_ >> double_;
	BOOST_SPIRIT_DEBUG_NODE(number_pair);

	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		(
			*(number_pair >> -qi::lit(';'))
		)
		, ascii::space, pairs);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse an '"} + std::string{it, s.end()} + "'"};

	for (auto & p : pairs)
		std::cout << p.first << ":" << p.second << "\n";

	std::cout << "done!\n";

	return 0;
}
