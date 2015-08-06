// std::string literal
#include <string>
#include <iostream>

int main(int argc, char * argv[])
{
	auto str = "hello"s;  // std::string literal
	std::cout << str.size() << "\n";
	return 0;
}

