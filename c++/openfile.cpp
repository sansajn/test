#include <string>
#include <fstream>
#include <iostream>
#include <cassert>

using std::string;
using std::ifstream;
using std::cout;
using std::cin;

int main(int argc, char * argv[])
{
	string ifile = argc > 1 ? argv[1] : "test.txt";
	ifstream fin{ifile};
	assert(fin.is_open());
	cout << "press <enter> to quit";
	string dummy;
	getline(cin, dummy);
	return 0;
}
