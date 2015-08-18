// test RTTI operatora typeid
#include <typeinfo>
#include <vector>
#include <string>
#include <iostream>

struct foo
{
	int i, j, k;
};


int main(int argc, char * argv[]) 
{
	std::cout << "vector<double>:" << typeid(std::vector<double>).name() << std::endl;
	std::cout << "double:" << typeid(double).name() << std::endl;
	std::cout << "string:" << typeid(std::string).name() << std::endl;
	std::cout << "foo:" << typeid(foo).name() << std::endl;
	return 0;
}
