#include <iostream>
#include <boost/utility/string_view.hpp>

using std::cout;
using boost::string_view;

void foo(string_view const & v)
{
	cout << v << "\n";
}

int main(int argc, char * argv[])
{
	char const * buf = "Hello John!"; 	
	foo(string_view{buf});
	cout << "done!\n";
	return 0;
}
