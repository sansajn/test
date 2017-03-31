#include <string>
#include <iostream>
#include <boost/variant.hpp>

using std::string;
using std::cout;

struct times_two_visitor : public boost::static_visitor<>
{
	void operator()(int & i) const {i *= 2;}
	void operator()(string & s) const {s += s;}
	void operator()(double & d) const {d *= 2.0;}
};

int main(int argc, char * argv[])
{
	using var = boost::variant<int, string, double>;

	var v1{"Hello!"};
	cout << v1 << std::endl;  // output: Hello!

	var v2{123};
	cout << v2 << std::endl;

	var v3{123.45};
	cout << v3 << std::endl;

	var v4;
	v4 = "John";
	cout << v4 << std::endl;

	var v5;
	v5 = 123.11;
	boost::apply_visitor(times_two_visitor{}, v5);
	cout << v5 << std::endl;

	return 0;
}
