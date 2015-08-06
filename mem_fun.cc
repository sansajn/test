/* g++ -o mem_fun mem_fun.cc */
#include <iostream>
#include <functional>
using namespace std;

struct foo
{
	void f1() {cout << "foo::f1() -> void\n";}
	int f2(double d) {cout << "foo::f2(double:" << d << ") -> int\n"; return 1;}
};

template <class Functor>
void invoker(Functor f) 
{
	foo o;
	f(o);
}

template <class Functor>
void invoker2(Functor f) 
{
	double d = 10.02;
}


int main()
{
	/* Nasledujúce dve funkcie vytvoria funktor ku ktorého valaniu 
	ale potrebujem objekt. */
	invoker(mem_fun_ref(&foo::f1));
	invoker(bind2nd(mem_fun_ref(&foo::f2), 10.01));

	foo o;
	invoker2(bind1st(mem_fun_ref(&foo::f2), o));


	return 0;
}
