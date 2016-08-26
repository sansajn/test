// implementuje modul emb s funkciou numargs() volatelnu zo skriptu
#include <string>
#include <iostream>
#include <Python.h>

using std::cout;
using std::string;

PyObject * emb_foo(PyObject * self, PyObject * args)
{
	cout << "hello from c++" << std::endl;
	cout << "is tuple " << PyTuple_Check(args) << std::endl;
	cout << "is exact tuple " << PyTuple_CheckExact(args) << std::endl;
	cout << "size (number of function arguments) " << PyTuple_Size(args) << std::endl;

	PyObject * dict_arg = PyTuple_GetItem(args, 3);  // borrowed reference
	cout << "is dict " << PyDict_Check(dict_arg) << "\n"
		<< "is exact dict " << PyDict_CheckExact(dict_arg) << "\n"
		<< "size " << PyDict_Size(dict_arg) << "\n";


	PyObject * foo_elem = PyDict_GetItemString(dict_arg, "foo");  // borrowed reference
	assert(foo_elem);
	assert(PyInt_Check(foo_elem) && "integer expected");
	long bar_number = PyInt_AsLong(foo_elem);

	PyObject * bar_elem = PyDict_GetItemString(dict_arg, "bar");  // borrowed reference
	assert(bar_elem);
	assert(PyString_Check(bar_elem) && "string expected");
	string bar_string = PyString_AsString(bar_elem);

	cout << "{'foo':" << bar_number << ", 'bar':'" << bar_string << "'}\n";

	Py_RETURN_NONE;
}

PyMethodDef EmbMethods[] = {
	{"foo", emb_foo, METH_VARARGS,
	"parse provided arguments"},
	{NULL, NULL, 0, NULL}
};

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();

	Py_InitModule("emb", EmbMethods);  // make the emb.numargs() function accessible

	PyRun_SimpleString(
		"import emb\n"
		"emb.foo((1,2,3,4), 'hello', 2, {'foo':5, 'bar':'five'})\n"
	);

	Py_Finalize();

	return 0;
}
