// string join algorithm sample
#include <vector>
#include <string>
#include <iostream>
#include <boost/algorithm/string/join.hpp>

using std::vector;
using std::string;
using std::cout;
using boost::algorithm::join;

int main(int argc, char * argv[])
{
	vector<string> data{"one", "two", "three"};
	string line = join(data, ", ");
	cout << line << "\n";
	return 0;
}
