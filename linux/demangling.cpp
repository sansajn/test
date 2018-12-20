// c++ abi identifiers demangling sample, abi::__cxa_demangle()
#include <exception>
#include <iostream>
#include <cxxabi.h>

using std::cout;
using std::type_info;

struct empty {};

template <typename T, int N>
struct bar {};


int main(int argc, char * argv[])
{
	// exception
	int status;
	std::bad_exception e;

	char * realname = abi::__cxa_demangle(e.what(), nullptr, 0, &status);
	if (status == 0)
		cout << e.what() << "\t => " << realname << "\n";
	else
		cout << "unable to unmangle '" << e.what() << "' (" << status << ")\n";

	free(realname);

	// typeid
	bar<empty, 17> u;
	type_info const & ti = typeid(u);
	realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	if (status == 0)
		cout << ti.name() << "\t => " << realname << "\n";
	else
		cout << "unable to unmangle '" << ti.name() << "' (" << status << ")\n";

	free(realname);

	return 0;
}
