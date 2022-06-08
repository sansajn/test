// string join algorithm sample
#include <vector>
#include <string>
#include <iostream>
#include <boost/algorithm/string/join.hpp>

using std::vector;
using std::string;
using std::cout;
using boost::algorithm::join;

int main(int argc, char * argv[]) {
	// joining from container
	vector<string> data{"one", "two", "three"};
	string line = join(data, ", ");
	cout << line << "\n";
	
	// joining from initializer-list not working :(
//	string line2 = join({"four", "five", "six"}, ",");
//	cout << line2 << "\n";
	
	cout << "done\n";
	return 0;
}
