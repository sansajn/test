// structured bindings sample
#include <map>
#include <string>
#include <iostream>

using std::map;
using std::string;
using std::cout;

int main(int argc, char * argv[]) {
	// from map
	map<int, string> m;
	m[1] = "one";
	m[2] = "two";
	m[5] = "five";
	
	for (auto & [key, value] : m)
		cout << "(" << key << ", " << value << ")\n";
	
	return 0;
}
