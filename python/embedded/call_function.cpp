// volá python funkciu uloženú v skript súbore
// spusti ako call_function call_function multiply
#include <Python.h>

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		printf("Usage: <call_function> python_source function_mame\n");
		return 1;
	}

	printf("args:\n");
	for (int i = 1; i < argc; ++i)
		printf("  %s\n", argv[i]);

	Py_Initialize();
	PySys_SetArgv(argc, argv);  // to work relative imports

	PyObject * pName = PyString_FromString(argv[1]);
	assert(pName && "runtime-error: nepodaril sa vytvorit python string");

	PyObject * pModule = PyImport_Import(pName);  // import py-script
	assert(pModule && "runtime-error: nepodaril sa importovat skript");

	PyObject * pDict = PyModule_GetDict(pModule);  // interpreter environment
	assert(pDict && "runtime-error: nepodaril sa vytvorit slovnik");

	// get py-script function
	PyObject * pFunc = PyDict_GetItemString(pDict, argv[2]);  
	assert(pFunc && "runtime-error: nepodarila sa ziskat funkcia zo skriptu");

	if (PyCallable_Check(pFunc))
		PyObject_CallObject(pFunc, NULL);  // volaj funkciu
	else
		PyErr_Print();

	// clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);

	Py_Finalize();

	return 0;
}
