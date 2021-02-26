#include <iostream>
#include <boost/format.hpp>

using std::cout;
using boost::format;

int main(int argc, char * argv[])
{
	float size1 = (10*1024 + 512)/1024.0f,
		size2 = 10*1024;
	cout << format("%1$.1fKB, %2$.1fKB") % size1 % size2 << "\n";  // posix printf style formating
	return 0;
}
