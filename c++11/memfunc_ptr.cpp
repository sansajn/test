// pouzitie ukazatela na clensku funkciu
#include <iostream>

using std::cout;

class foo
{
public:
	void f1() {cout << "foo::f1()\n";}
};

using memptr_t = void(foo::*)();

void dot_call(foo & f)
{
	memptr_t func = &foo::f1;
	(f.*func)();
}

void arrow_cal(foo * f)
{
	memptr_t func = &foo::f1;
	(f->*func)();
}

int main(int argc, char * argv[])
{
	foo f;
	dot_call(f);
	arrow_cal(&f);
	return 0;
}