#include <string>
#include <iostream>
#include <boost/regex.hpp>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string t = "Jane,33";
	boost::regex e{R"((\w+),(\d+))"};
	boost::smatch what;
	if (boost::regex_match(t, what, e))
	{
		for (int i = 0; i < what.size(); ++i)
			cout << "  $" << i+1 << "=" << what[i] << "\n";
	}
	return 0;
}