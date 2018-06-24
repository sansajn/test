// read line from file sample
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

using std::string;
using std::getline;
using std::ifstream;
using std::cout;

int main(int argc, char * argv[])
{
	ifstream fin{"SConstruct"};
	assert(fin.is_open());
	
	string line;
	while (getline(fin, line))
		cout << line << "\n"; 
	
	return 0;
}
