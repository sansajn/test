// see wg21.link/n3649
#include <cassert>
#include <cstring>

int main(int argc, char * argv[])
{
	auto identity = [](auto x){return x;};
	int three = identity(3);
	char const * hello = identity("hello");
	
	assert(three == 3);
	assert(strcmp(hello, "hello") == 0);
	
	// conversion to function pointer
	int (*fpi)(int) = identity;
	char (*fpc)(char) = identity;
	
	return 0;
}