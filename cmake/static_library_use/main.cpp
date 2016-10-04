#include <iostream>
#include "version.hpp"

int main(int argc, char * argv[])
{
	std::cout << "version: " << version() << std::endl;
	return 0;
}
