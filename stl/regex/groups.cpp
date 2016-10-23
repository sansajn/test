// simple groups test
#include <regex>
#include <string>
#include <iostream>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string t = "Jane,33";
	std::regex e{R"((\w+),(\d+))"};
	std::smatch what;
	if (std::regex_match(t, what, e))
	{
		for (size_t i = 0; i < what.size(); ++i)
			cout << "  $" << i+1 << "=" << what[i] << "\n";
	}
	return 0;
}
