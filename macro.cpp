// g++ macro.cpp -o macro
#include <iostream>
using std::cout;

#define PTDEBUG defined _DEBUG && defined _PTDEBUG

#define _DEBUG

#define _PTDEBUG

int main(int argc, char * argv[])
{
#if PTDEBUG
	cout << "in debug\n";
#endif
	cout << "done!\n";
	return 0;
}
