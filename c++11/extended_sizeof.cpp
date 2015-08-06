// extended sizeof
#include <iostream>

struct s
{
	int i;
	char c;
};

int main(int argc, char * argv[])
{
	std::cout << sizeof(s::c) << std::endl;
}
