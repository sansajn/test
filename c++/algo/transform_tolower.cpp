// transform algorithm sample: transform given string to lower case
#include <algorithm>
#include <string>
#include <iostream>
using std::transform, std::string, std::cout;

int main(int argc, char * argv[])
{
	string s = "HeLLo!";
	transform(begin(s), end(s), begin(s), ::tolower);  //! ::tolower is not std::tolower
	cout << s << "\n";
	return 0;
}
