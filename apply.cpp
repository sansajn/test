// apply implementation sample (prevent object mutating)
#include <iostream>
using std::ostream, 
	std::cout;

class selection
{
public:
	selection() : _a{0}, _b{0}, _c{0} 
	{}

	selection(selection const & other)
		: _a{other._a}, _b{other._b}, _c{other._c}
	{}

	struct applyer 
	{
		applyer(selection & sel) : _sel{sel} {}
		applyer & a(int val) {_sel._a = val; return *this;}
		applyer & b(int val) {_sel._b = val; return *this;}
		applyer & c(int val) {_sel._c = val; return *this;}
		operator selection() {return _sel;}

		selection & _sel;
	};

	applyer apply() {return applyer{*this};}

	friend ostream & operator<<(ostream & out, selection const & sel)
	{
		out << "(a=" << sel._a << ", b=" << sel._b << ", c=" << sel._c << ")";
		return out;
	}

private:
	int _a, _b, _c;
};

int main(int argc, char * argv[])
{
	selection s1;
	
	cout << "s1=" << s1 << "\n";

	selection s2 = selection(s1).apply()
		.a(15)
		.c(10);

	cout << "s2=" << s2 << "\n"
		<< "done!\n";

	return 0;
}
