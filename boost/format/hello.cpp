#include <iostream>
#include <boost/format.hpp>

using std::cout;

int main(int argc, char * argv[])
{
	cout << boost::format("writing %1%, x=%2% : %3%-th try") % "toto" % 40.23 % 50 << "\n";
	return 0;
}
