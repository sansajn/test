// read python variable
#include <iostream>
#include <boost/python.hpp>

using std::cout;
namespace py = boost::python;

int main(int argc, char * argv[])
{
	Py_Initialize();

	py::object main_module = py::import("__main__");
	py::object main_namespace = main_module.attr("__dict__");
	py::exec("result = 5 ** 2", main_namespace);
	int result = py::extract<int>(main_namespace["result"]);
	cout << "5**2=" << result << std::endl;

	Py_Finalize();
	
	cout << "done!\n";
	return 0;
}
