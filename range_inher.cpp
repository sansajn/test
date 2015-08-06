#include <iostream>
using std::cout;

struct foo 
{
public:
	foo() : _i(0) {}

	void operator++() {
		_i++;
	}

	int & operator*() {
		return _i;
	}

	operator bool() const {
		return _i < 10;
	}

private:
	int _i;
};

struct goo : public foo {	
};


// unary operator
template <class R, class Condition>
struct filter
{
public:
	filter(R g) : _g(g), _cond() {}
	filter(R g, Condition c) : _g(g), _cond(c) {}

	void operator++() {		
		do {
			++_g;
		} while (!_cond(*_g));
	}

	int & operator*() {  // toto je štandardizovane
		return *_g;
	}

	operator bool() const {  // toto je tiež štandardne
		return bool(_g);
	}

private:
	R _g;
	Condition _cond;
};


struct divide_by_two {
	bool operator()(int const & x) const {
		return (x % 2) == 0;
	}
};

goo make_goo() {
	return goo();	
}


int main(int argc, char * argv[])
{
	for (goo g; g; ++g)
		cout << *g << " ";

	cout << "\n";

	filter<goo, divide_by_two> f(make_goo());
	for (f; f; ++f)
		cout << *f << " ";

	cout << "\n";

	return 0;
}

