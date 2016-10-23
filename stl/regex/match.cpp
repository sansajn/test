// simple regex_match test
#include <regex>
#include <string>
#include <iostream>

using std::cout;
using std::string;

int main(int argc, char * argv[])
{
	std::regex pat{R"([_[:alpha:]]\w*)"};  // c++ identifier

	string inputs[] = {"hello", "_hello", "21_hello", "H21ello", "@__hello", "hell o"};

	for (string const & in : inputs)
	{
		if (std::regex_match(in, pat))
			cout << in << " is a c++ identifier\n";
		else
			cout << in << " is not a c++ identifier\n";
	}

	return 0;
}
