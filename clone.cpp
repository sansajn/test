#include <iostream>
using std::cout;

class foo
{
public:
	void set(int val) {_val = val;}
	int get() const {return _val;}
	virtual foo * clone() const = 0;

protected:
	void copy_to(foo * dst) const {
		dst->_val = _val;
	}

/*
	foo & operator=(foo const & rhs) {
		_val = rhs._val;
	}
*/

private:
	int _val;
};

class goo 
	: public foo
{
public:
	goo * clone() const {
		goo * g = new goo;
		foo::copy_to(g);
		return g;		
	}
};


int main(int argv, char * argc[])
{
	goo g;
	g.set(101);

	foo * f = g.clone();

	cout << "f::get()=" << f->get() << "\n";

	foo * h = f->clone();

	cout << "h::get()=" << h->get() << "\n";

	foo k;  // nejde, abstraktna klasa
	
	return 0;
}


