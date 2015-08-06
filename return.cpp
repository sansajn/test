#include <iostream>
using std::cout;

struct foo
{
	~foo() {cout << "~foo()\n";}
};

foo goo()
{
	return foo();
}

int main(int argc, char * argv[])
{
	{
		foo f = goo();
		cout << "done!";
	}
	return 0;
}

