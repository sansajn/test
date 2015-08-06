#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <cassert>

using std::string;
using std::ifstream;
using std::cout;
using std::move;

/*
ifstream open(string const & fname)
{
	ifstream fin(fname);
	assert(fin.is_open() && "unable to open a file");
	return std::move(fin);
}
*/

int main(int argc, char * argv[])
{
	// ifstream fin = open("test.txt");
	ifstream _fin("test.txt");
	ifstream fin(std::move(_fin));  // stream nema implmentovanu move semantiku (v gcc)
	string s;
	fin >> s;
	cout << s << std::endl;
	fin.close();
	return 0;
}
