// expose structureto python program
#include <string>
#include <boost/python.hpp>

using std::string;
namespace py = boost::python;

struct World
{
	string msg;
	void set(string msg) {this->msg = msg;}
	string greet() {return msg;}
};

BOOST_PYTHON_MODULE(hello)
{
	py::class_<World>("World")
		.def("greet", &World::greet)
		.def("set", &World::set)
	;
}


int main(int argc, char * argv[])
{
	Py_Initialize();
	try {
		inithello();  // initialize hello module
		PyRun_SimpleString("import hello");
		PyRun_SimpleString("planet = hello.World()");
		PyRun_SimpleString("planet.set('howdy')");
		PyRun_SimpleString("print planet.greet()");
	}
	catch (py::error_already_set) {
		PyErr_Print();
	}
	Py_Finalize();
	return 0;
}
