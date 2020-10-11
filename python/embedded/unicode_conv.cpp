// python (unicode) string conversion to UTF8 encoded c-string sample
#include <string>
#include <iostream>
#include <Python.h>

using std::cout, std::endl;

PyObject * emb_foo(PyObject * self, PyObject * args)
{
	PyObject * utf8_str = PyTuple_GetItem(args, 0);
	assert(utf8_str && PyUnicode_Check(utf8_str));

	cout << "unicode: " << PyUnicode_Check(utf8_str) << "\n"
		<< "string: " << PyString_Check(utf8_str) << endl;

	PyObject * str = PyUnicode_AsUTF8String(utf8_str);
	assert(str && PyString_Check(str));

	char const * buf = PyString_AsString(str);
	assert(str);

	cout << buf << endl;

	size_t len = strlen(buf);

	Py_DECREF(str);

	return Py_BuildValue("i", len);
}

PyMethodDef emb_method_list[] = {
	{"foo", emb_foo, METH_VARARGS,
	"Return the number of arguments received by the process."},
	{nullptr, nullptr, 0, nullptr}
};

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();

	Py_InitModule("emb", emb_method_list);  // make the emb.numargs() function accessible

	PyRun_SimpleString(
		"import emb\n"
		"len = emb.foo(u'hello ľščťžýáíé characters')\n"
		"print('string is %d bytes length' % len)\n"
		"print('done!')\n"
	);

	Py_Finalize();

	return 0;
}
