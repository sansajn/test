#include <string>
#include <vector>
#include <iostream>
#include <boost/variant.hpp>

using std::string;
using std::vector;
using std::cout;

struct times_two_visitor : public boost::static_visitor<>
{
	template <typename T>
	void operator()(T & x) const {x += x;}
};

struct print_visitor : public boost::static_visitor<>
{
	template <typename T>
	void operator()(T & x) const {cout << x << "\n";}
};


int main(int argc, char * argv[])
{
	using var = boost::variant<int, string, double>;

	vector<var> vars{"hello!", 13.11, 12};

	times_two_visitor visitor;
	std::for_each(vars.begin(), vars.end(), boost::apply_visitor(visitor));

	print_visitor printer;
	std::for_each(vars.begin(), vars.end(), boost::apply_visitor(printer));

	return 0;
}