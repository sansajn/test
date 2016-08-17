// implementuje modul emb s funkciou numargs() volatelnu zo skriptu
#include <Python.h>

int numargs = 0;

PyObject * emb_numargs(PyObject * self, PyObject * args)
{
	if (!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return Py_BuildValue("i", numargs);
}

PyMethodDef EmbMethods[] = {
	{"numargs", emb_numargs, METH_VARARGS,
	"Return the number of arguments received by the process."},
	{NULL, NULL, 0, NULL}
};

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();
	
	numargs = argc;
	Py_InitModule("emb", EmbMethods);  // make the emb.numargs() function accessible
	
	PyRun_SimpleString(
		"import emb\n"
		"print('Number of arguments', emb.numargs())\n"
	);
	
	Py_Finalize();
	
	return 0;
}