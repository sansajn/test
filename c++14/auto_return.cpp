// auto return type
// g++ <file> -std=c++1y -o <output>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

auto foo()
{
	std::map<int, std::vector<std::pair<int, std::string>>> vec;
	return vec;
}

template<class T1, class T2> auto goo(T1 a, T2 b)
{
	return a + b;
}

int main(int argc, char * argv[])
{
	foo();
	std::cout << goo(4, 5.4) << "\n";

	return 0;
}

