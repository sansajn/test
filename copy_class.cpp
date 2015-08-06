#include <iostream>
using std::cout;

struct foo
{
	int * data;

	foo() {cout << "foo::foo():" << long(this) << "\n";}
	foo(foo & f) {cout << "foo::foo(foo &):" << long(this) << "\n";}
	foo(foo const & f) {cout << "foo::foo(foo const &):" << long(this) << "\n";}
	~foo() {cout << "foo::~foo():" << long(this) << "\n";}

	void dummy() {cout << "foo::dummy():" << long(this) << "\n";}
};

void func(foo f)
{
	f.dummy();
}

void func_ref(foo & f)
{
	f.dummy();
}


int main(int argc, char * argv[])
{
	{
		cout << "test1\n";
		func(foo());
	}

	{
		cout << "test2\n";
		foo f;
		func(f);
	}

	{
		cout << "test3\n";
		foo g;
		func_ref(g);
	}

	return 0;
}
