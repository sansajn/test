// reverse algorithm sample: reverse given string
#include <algorithm>
#include <string>
#include <iostream>

using std::string, std::cout, std::reverse;

int main(int argc, char * argv[])
{
	string s = "hello!";
	reverse(begin(s), end(s));
	cout << s << "\n";
	return 0;
}
