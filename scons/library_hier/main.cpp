#include <iostream>
#include <string>
#include <foo/about.hpp>
#include <goo/utils.hpp>

using std::cout;
using std::string;

int main(int argc, char * argv[])
{
	about a;
	cout << a.version() << "\n"
		<< a.author() << "\n";
	utils u;
	string s("/////hello///my//name");
	cout << "number of '/' in '" << s << "' is " << u.count(s) << "\n";
	return 0;
}
