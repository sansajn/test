// ulozenie zoznamu do vektora pomocou atrubutu parse funkcie
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace ascii = boost::spirit::ascii;
using boost::spirit::qi::double_;

int main(int argc, char * argv[])
{
	std::string s = "11, 14, -2, 3.14";

	std::vector<double> vals;
	
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(), double_ % ',', ascii::space, vals);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};

	std::copy(vals.begin(), vals.end(), std::ostream_iterator<double>(std::cout, ", "));

	std::cout << "\ndone!\n";
	return 0;
} 
