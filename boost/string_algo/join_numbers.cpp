// string join algorithm sample
#include <vector>
#include <string>
#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

using std::vector;
using std::string;
using std::to_string;
using std::cout;
using boost::algorithm::join;
using boost::adaptors::transformed;


int main(int argc, char * argv[])
{
	vector<int> data{1, 2, 3};
	
	// with lambda
	string line = join(data|transformed(
		[](int v){
			return to_string(v);
		}), ", ");
	
	cout << line << "\n";
	
	
	// with to_string
	line = join(data|transformed(
		static_cast<string (*)(int)>(to_string)), ", ");
	
	cout << line << "\n";
	
	return 0;
}
