// try_emplace() member sample for map

#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
using std::map, std::string, std::move, std::make_pair, std::cout;

int main(int argc, char * argv[])
{
	map<string, string> m;
	m["Hello"] = "World";

	string s = "C++";
	m.emplace(make_pair("Hello", move(s)));  // s should be empty after emplace call

	// what happens with the string 's'?
	cout << s << "\n"              // ""
		<< m["Hello"] << "\n";      // "World"

	s = "C++";
	m.try_emplace("Hello", move(s));
	cout << s << "\n"
		<< m["Hello"] << "\n";

	cout << "done!\n";

	return 0;
}
