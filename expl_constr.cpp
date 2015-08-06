#include <cmemory>

struct MyClass
{
	int i;
};


int main()
{
	MyClass * mine = (MyClass *)malloc(sizeof(MyClass));  // Allocates memory
	mine->MyClass();                           // Calls constructor
	return 0;
}

