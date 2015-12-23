// dedicnost sablovy s sablonovym parametrom
#include <iostream>

using std::cout;

template <template<class> class A, typename B>
class foo : public A<B>
{
	// funkcia goo::funct_a() je dostupna v foo
};

template <typename T>
class goo : public T
{
public:
	void funct_a() {cout << "goo<>::funct_a()" << std::endl;}
};

class hoo {};

int main(int argc, char * argv[])
{
	foo<goo, hoo> f;
	f.funct_a();

	foo<goo, hoo> * p = new foo<goo, hoo>{};
	p->funct_a();

	return 0;
}
