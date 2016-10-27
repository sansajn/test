// read python variable (note; calling exec_file() causing crash, probably mixing library versions)
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/python.hpp>

using std::cout;
using std::string;
using std::ifstream;
using std::stringstream;
namespace py = boost::python;

string read_file(string const & fname);

int main(int argc, char * argv[])
{
	Py_Initialize();

	py::object main_module = py::import("__main__");
	py::object main_namespace = main_module.attr("__dict__");
	// py::exec_file("app_conf.py", main_namespace);
	string content = read_file("app_conf.py");
	py::exec(content.c_str(), main_namespace);

	string name = py::extract<string>(main_namespace["name"]);
	int weight = py::extract<int>(main_namespace["weight"]);
	double height = py::extract<double>(main_namespace["height"]);
	short age = py::extract<short>(main_namespace["age"]);
	bool is_mentlist = py::extract<bool>(main_namespace["mentalist"]);

	cout << "name:" << name << "\n"
		<< "weight:" << weight << "\n"
		<< "height:" << height << "\n"
		<< "age:" << age << "\n"
		<< "mentlist:" << is_mentlist << std::endl;

	Py_Finalize();
	
	cout << "done!\n";
	return 0;
}

string read_file(string const & fname)
{
	ifstream in{fname};
	stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}