// optimalizacia r-reference
#include <iostream>

using namespace std;

class foo
{
public:
	int x;
	
	foo() {cout << "foo()" << endl;}
	~foo() {cout << "~foo()" << endl;}

	foo(foo const &) = delete;
	//foo(foo const & rhs) : x(rhs.x) {cout << "foo(foo&)" << endl;}
	
	/*
	foo & operator=(foo & rhs)
	{
		x = rhs.x;
		cout << "foo::operator=(foo&)" << endl;
		return *this;
	}
	*/
	
	foo(foo && rhs) : x(rhs.x) {cout << "foo(foo&&)" << endl;}
	
	/*
	foo & operator=(foo && rhs)
	{
		x = rhs.x;
		cout << "foo::operator=(foo&&)" << endl;
		return *this;
	}
	*/
};

static foo func();

int main(int argc, char * argv[])
{
	foo f = func();
	cout << "f.x:" << f.x << "\n";
	cout << "done\n";
	return 0;
}

foo func()
{
	foo f;
	f.x = 101;
	cout << "func()" << endl;
	return f;
}