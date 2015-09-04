/* Example 11.13. Defining rules with boost::spirit::qi::rule
http://theboostcpplibraries.com/boost.spirit-rules#ex.spirit_13 */
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit;

int main()
{
	std::string s;
	std::getline(std::cin, s);
	auto it = s.begin();
	qi::rule<std::string::iterator, std::vector<int>(), 
		ascii::space_type> values = qi::int_ % ',';
	std::vector<int> v;
	if (qi::phrase_parse(it, s.end(), values, ascii::space, v))
		std::copy(v.begin(), v.end(), std::ostream_iterator<int> {std::cout, ";"});
	std::cout << "\n";
}
