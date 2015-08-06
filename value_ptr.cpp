// kedy sa destruuje docasny objekt
#include <iostream>
using std::cout;

class foo
{
public:
	foo() {cout << "foo()\n";}
	~foo() {cout << "~foo()\n";}
};

foo const * value_ptr(foo const & f) {return &f;}

void use_foo_ref(foo const * f)
{
	cout << "use_foo_ref(foo const * f)\n";
}


int main(int argc, char * argv[])
{	
	use_foo_ref(value_ptr(foo()));
	return 0;
}
