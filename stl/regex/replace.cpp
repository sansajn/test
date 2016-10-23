// world space number sample
#include <regex>
#include <string>
#include <iostream>

using std::cout;
using std::string;

int main(int argc, char * argv[])
{
	string input{"x 1 y2 22 zaq 34567"};
	std::regex pat{R"((\w+)\s(\d+))"};
	string format{"{$1,$2}\n"};
	cout << std::regex_replace(input, pat, format);
	return 0;
}
