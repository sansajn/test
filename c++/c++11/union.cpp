#include <string>
#include <iostream>

using std::string;
using std::cout;

union var  // union with non trivial type
{
	string str;
	int num;
	var() {}
	~var() {}
};


int main(int argc, char * argv[]) 
{
	var v1;
	v1.str = "Hello!";
	cout << v1.str << std::endl;

	var v2;
	v2.num = 123;
	cout << v2.num << std::endl;

	return 0;
}
