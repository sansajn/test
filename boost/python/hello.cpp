// most simple expose
#include <boost/python.hpp>

using namespace boost::python;

char const * greet()
{
	return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
	def("greet", greet);
}

int main(int argc, char * argv[])
{
	Py_Initialize();

	try {
		inithello_ext();  // initialize hello_ext
		PyRun_SimpleString("import hello_ext");
		PyRun_SimpleString("print hello_ext.greet()");
	}
	catch (error_already_set) {
		PyErr_Print();
	}

	Py_Finalize();

	return 0;
}
