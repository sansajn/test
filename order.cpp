#include <iostream>
using std::cout;

class joo 
{
public:
	joo() {cout << "joo()\n";}
};

class foo 
{
public:
	foo() {cout << "foo()\n";}
	joo & get() {return j;}

private:
	joo j;
};

class goo 
{
public:
	goo(joo & j) {cout << "goo(j:" << (void *)&j << ")\n";}
};

class hoo 
{
public:
	hoo() : g(f.get()) {cout << "hoo()\n";}

private:
	foo f;
	goo g;	
};


int main(int argc, char * argv[])
{
	hoo h;
	return 0;
}
