#include <string>
#include <iostream>
#include "operator_override.hpp"
using std::string;
using std::cout;

namespace sub = test::sub;

namespace test {
	namespace sub {

inline foo & operator<<(foo & f, string const & s)
{
	cout << "operator<<(foo & f, string const &)\n";
	return f;
}

inline void operator<<(foo && f, string const & s)
{
	cout << "operator<<(foo && f, string const &)\n";
}

	}  // sub
} // test


int main(int argc, char * argv[])
{
	sub::foo f;
	f << 15;
	f << string("hello!");

	sub::foo() << 20;
	sub::foo() << string("hello!");  // vyžaduje konštantný operator

	return 0;
}
