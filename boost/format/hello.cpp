/*! \file hello.cpp
basic formating with boost::format */

#include <string>
#include <iostream>
#include <boost/format.hpp>

using std::string;
using std::cout;
using boost::format;
using boost::str;

int main(int argc, char * argv[])
{
	string s = str(format("hello %1%") % "John");
	cout << format("writing %1%, x=%2% : %3%-th try") % "toto" % 40.23 % 50 << "\n";
	return 0;
}
