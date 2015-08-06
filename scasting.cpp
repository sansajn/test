#include <iostream>
#include <cstdint>
using std::cout;

int main(int argc, char * argv[])
{
	char const * s = "help!";
	intptr_t param = intptr_t(s);
//	char const * p = static_cast<char const *>(param);
//	char const * p = (char const *)(param);
	char const * p = reinterpret_cast<char const *>(param);
	cout << "p:" << p << "\n";
	return 0;
}

