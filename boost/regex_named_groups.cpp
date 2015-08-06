#include <string>
#include <iostream>
#include <boost/regex.hpp>

using std::string;
using std::cout;

int main(int argc, char * argv[])
{
	string t = "Jane,33";
	boost::regex e{R"((?'name'\w+),(?'year'\d+))"};
	boost::smatch what;
	if (boost::regex_match(t, what, e))
	{
		string name = what["name"];  // what[] vracia sub_match
		string year = what["year"];		
		cout << "(" << name << ", " << year << ")\n";
	}
	return 0;
}