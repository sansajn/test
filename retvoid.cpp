#include <iostream>

void foo() {std::cout << "foo()\n";}
void goo() {std::cout << "goo()\n"; return foo();}


int main()
{
	goo();  // prints 'goo()\nfoo()'
	return 0;
}
