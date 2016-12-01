// implementuje modul emb s funkciou numargs() volatelnu zo skriptu
#include <Python.h>

int numargs = 0;

PyObject * emb_numargs(PyObject * self, PyObject * args)
{
	// TODO: ako vypada args n-tica ?
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return Py_BuildValue("i", numargs);
}

PyObject * emb_version(PyObject * self, PyObject * args)
{
	assert(!args && "null expected");
	return Py_BuildValue("s", "0.0.1.0.0");
}

PyMethodDef emb_method_list[] = {
	{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
	{"version", emb_version, METH_NOARGS, "Return module version as a string"},
	{NULL, NULL, 0, NULL}
};

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();
	
	numargs = argc;
	Py_InitModule("emb", emb_method_list);  // make the emb.numargs() function accessible
	
	PyRun_SimpleString(
		"import emb\n"
		"print('Number of arguments', emb.numargs())\n"
		"print('module version', emb.version())\n"
		"print('done')\n"
	);
	
	Py_Finalize();
	
	return 0;
}
