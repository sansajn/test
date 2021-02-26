#include <iostream>
#include <boost/format.hpp>

using std::cout;
using boost::format;

int main(int argc, char * argv[])
{
	cout << "11: " << format("%1$03d") % 11 << "\n";
	cout << "1: " << format("%1$03d") % 1 << "\n";
	
	// pipe delimited syntax (no need for type-conversion character)
	cout << format("%|1$03|") % 1 << "\n"
		<< format("%|1$03|") % 11 << "\n"
		<< format("%|1$03|") % 112 << "\n";

	cout << format("%|1$| - %|2$03|") % "hello" % 11 << "\n";

	return 0;
}
