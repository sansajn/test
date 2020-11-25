// exception_ptr sample

#include <exception>
#include <string>
#include <iostream>

using std::exception_ptr; 
using std::current_exception; 
using std::rethrow_exception; 
using std::exception;
using std::string;
using std::cout;

void handle_exception(exception_ptr e)
{
	try {
		if (e)
			rethrow_exception(e);
	} 
	catch (exception const & e) {
		cout << "caught exception '" << e.what() << "'\n";
	}
}

int main(int argc, char * argv[])
{
	exception_ptr e;
	try {
		string{}.at(1);  // this generates an out_of_range exception
	}
	catch(...) {
		e = current_exception();
	}

	handle_exception(e);

	return 0;
}
