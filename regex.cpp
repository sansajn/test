//#include <regex>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
//using std::regex;
using std::string;
using std::cout;
using boost::regex;


int main(int argc, char * argv[])
{
	regex r(argv[1]);
	string s = "5";
	if (regex_match(s, r))
		cout << "matched\n";
	else
		cout << "not matched\n";
	return 0;
}

