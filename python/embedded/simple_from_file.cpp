// vykona jednoduchy python program ulozeny ako subor
#include <Python.h>

char const * script_path = "simple_script.py";

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();
	
	FILE * fp = fopen(script_path, "r");
	assert(fp);
	
	PyRun_SimpleFile(fp, script_path);
	
	fclose(fp);
	Py_Finalize();
		
	return 0;
}
