#include <algorithm>
#include <string>
#include <cctype>
#include <iostream>

using namespace std;

void _tolower(string & s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
}


int main(int argc, char * argv[])
{
	string s("HeLLo");
	_tolower(s);
	cout << s << "\n";
	return 0;
}

