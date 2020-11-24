// inline class variable static member sample

#include <string>
#include <iostream>
using std::string;
using std::cout;

struct foo
{
	static inline string name = "Peter";
};

int main()
{
	cout << foo::name << "\n"
		<< "done!\n";
	return 0; 
}