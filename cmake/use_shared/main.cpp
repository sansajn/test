#include <iostream>
#include "version.hpp"

using std::cout;


int main(int argc, char * argv[])
{
	cout << version() << "\n"
		<< "done!\n";
	
	return 0;
}