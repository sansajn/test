// move-nutie hierarchie
#include <iostream>
#include <algorithm>

class foo
{
public:
	foo(int i) : _dummy(i) {std::cout << "foo()" << std::endl;}
	virtual ~foo() {std::cout << "~foo()" << std::endl;}

	int get() const {return _dummy;}
	
	foo(foo && lhs) {
		_dummy = lhs._dummy;
		lhs._dummy = 0;
		std::cout << "foo(foo &&)" << std::endl;
	}

	void operator=(foo && lhs) {
		std::swap(_dummy, lhs._dummy);
		std::cout << "operator=(foo &&)" << std::endl;
	}

	foo(foo const &) = delete;
	void operator=(foo const &) = delete;
	
private:
	int _dummy;
};

class goo : public foo
{
public:
	goo(int i) : foo(i) {std::cout << "goo()" << std::endl;}
	~goo() {std::cout << "~goo()" << std::endl;}

	goo(goo && lhs) : foo(std::move(lhs)) {
		std::cout << "goo(goo &&)" << std::endl;
	}

	void operator=(goo && lhs) {
		foo::operator=(std::move(lhs));
		std::cout << "operator=(goo &&)" << std::endl;
	}

	goo(goo const &) = delete;
	void operator=(goo const &) = delete;
};

goo create();

int main(int argc, char * argv[])
{
	goo g(1);
	g = create();
	std::cout << "g.get()=" << g.get() << std::endl;
	return 0;
}

goo create()
{
	goo g(2);
	g.get();
	return g;
}
