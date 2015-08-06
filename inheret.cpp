/* dedičnosť operátora = */
#include <iostream>
using std::cout;

class foo
{
public:
	foo() : x(15) {}
	int const & value() const {return x;}

private:
	void operator=(foo const &);

protected:
	int x;
};

class goo
	: public foo
{
public:
	goo() {}

	goo & operator=(foo const & b) {
		x = b.value();
		return *this;
	}

	goo & operator=(goo const & b) {
		x = b.value();
		return *this;
	}

	void reset(int v) {x=v;}
};


int main()
{
	goo g;
	cout << g.value() << "\n";
	return 0;
}
