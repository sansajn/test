// implementacia terminate-handlera
#include <exception>
#include <typeindex>
#include <iostream>
#include <cstdlib>
#include <cxxabi.h>

using std::cerr;
using std::abort;
using std::exception;
using std::type_info;

void verbose_terminate_handler()  // taken from libsupc++ (vterminate.cc)
{
	static bool terminating = false;
	if (terminating)
	{
		cerr << "terminate called recursively\n";
		abort();
	}
	terminating = true;
	
	type_info * t = __cxxabiv1::__cxa_current_exception_type();
	if (t)
	{
		char const * name = t->name();  // mangled name
		{
			int status = -1;
			char * dem = __cxxabiv1::__cxa_demangle(name, 0, 0, &status);
			cerr << "terminate called after throwing an instance of '" << (status == 0 ? dem : name) << "'\n";
			if (status == 0)
				free(dem);
		}
		
		// if derived from std::excepton, we can give more information
		try { throw; }
		catch (exception const & e) {
			cerr << "  what():  " << e.what() << std::endl;
		}
		catch(...) {}
	}
	else
		cerr << "terminate called without an active exception" << std::endl;
	
	abort();
}

int main(int argc, char * argv[])
{
	std::set_terminate(verbose_terminate_handler);
	throw std::logic_error{"nejaka chyba ..."};
	return 0;
}
