#include <sstream>
#include <iostream>
#include <string>

int main(int argc, char * argv[])
{
	std::ostringstream ostr;
	ostr << "hello";
	std::string t1 = ostr.str();

	ostr.str("");
	ostr << "Peter";
	std::string t2 = ostr.str();

	std::cout << "t1:" << t1 << ", t2:" << t2 << std::endl;
	return 0;
}
