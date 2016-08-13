// volá python funkciu uloženú v skript súbore
// spusti ako call_function call_function multiply
#include <string>
#include <iostream>
#include <cassert>
#include <Python.h>

using std::string;
using std::cout;

void call_function(int argc, char * argv[], string const & module_name, string const & function_name)
{
	double x = 10.01;

	cout << "calls " << module_name << "." << function_name << "(" << x << ")" << std::endl;

	Py_Initialize();
	PySys_SetArgv(argc, argv);  // to work relative imports

	PyObject * name = PyString_FromString(module_name.c_str());
	assert(name && "runtime-error: nepodaril sa vytvorit python string");

	PyObject * module = PyImport_Import(name);  // import py-script
	assert(module && "runtime-error: nepodaril sa importovat skript");

	PyObject * env = PyModule_GetDict(module);  // interpreter environment
	assert(env && "runtime-error: nepodaril sa vytvorit slovnik");

	// get py-script function
	PyObject * func = PyDict_GetItemString(env, function_name.c_str());  // borrowed reference (do not decref)
	assert(func && "runtime-error: nepodarila sa ziskat funkcia zo skriptu");

	// prepare function's attributes
	PyObject * args = PyTuple_Pack(1, PyFloat_FromDouble(x));

	if (PyCallable_Check(func))
	{
		PyObject * result = PyObject_CallObject(func, args);  // volaj funkciu
		assert(result);
		double y = PyFloat_AsDouble(result);
		cout << "resuslt:" << y << std::endl;
		Py_DECREF(result);
	}
	else
		PyErr_Print();

	// clean up
	Py_DECREF(args);
	Py_DECREF(env);
	Py_DECREF(module);
	Py_DECREF(name);

	Py_Finalize();
}

int main(int argc, char * argv[])
{
	string module_name = argc > 1 ? argv[2] : "test";
	string function_name = argc > 2 ? argv[3] : "multiply_with_arg";

	for (int i = 0; i < 10; ++i)
	{
		cout << "i=" << i << std::endl;
		call_function(argc, argv, module_name, function_name);
	}

	return 0;
}
