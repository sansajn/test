/* g++ path.cpp -o path -lboost_filesystem -lboost_system */
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;

int main(int argc, char * argv[])
{
	path p("/home/ja");
	p /= "temp";
	cout << p << "\n";
	string s = p.c_str();
	cout << s << "\n";
	return 0;
}

