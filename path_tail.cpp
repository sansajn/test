#include <string>
#include <iostream>

using std::cout;
using std::string;

int main(int argc, char * argv[]) 
{
	string path{"a/b/c"};

	string tail;
	string::size_type pos = path.rfind('/');
	if (pos != string::npos && pos < path.size()-1)
		tail = path.substr(pos+1);
	
	cout << tail << "\n";

	return 0;
}
