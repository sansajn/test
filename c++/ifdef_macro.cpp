#include <iostream>

using std::cout;

#ifdef NDEBUG
#define TEST_ALIAS
#endif

int main(int argc, char * argv[]) {
#ifdef TEST
	cout << "TEST defined\n";
#endif
	
#ifdef TEST_ALIAS
	cout << "TEST_ALIAS defined\n";
#else
	cout << "TEST_ALIAS not defined\n";
#endif
	
	cout << "done.\n";
	return 0;
}
