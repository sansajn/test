/* let's parse !
http://www.boost.org/doc/libs/1_59_0/libs/spirit/doc/html/spirit/qi/tutorials/warming_up.html */
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <boost/spirit/include/qi.hpp>

using std::string;
using std::cout;
using std::cin;
using std::logic_error;
using boost::spirit::qi::double_;
using boost::spirit::qi::phrase_parse;
using boost::spirit::ascii::space;


int main(int argc, char * argv[])
{
	string s;
	getline(cin, s);

	auto it = s.begin();
	bool r = phrase_parse(it, s.end(), 
		double_ >> *(',' >> double_),
		space);

	if (!r || it != s.end())
		throw logic_error{string{"unable to parse '"} + string{it, s.end()} + "'"};

	cout << "done\n";

	return 0;
}