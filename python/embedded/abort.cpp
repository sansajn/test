// ukonci prebiehajuce python volanie

#include <thread>
#include <iostream>
#include <Python.h>

using std::thread;
using std::cout;

void thread_func()
{
	PyRun_SimpleString(
		"print('wait ...')\n"
		"while True:\n"
		"   continue\n"
		"print('done')\n"
	);
}

int quit(void *)
{
	PyErr_SetInterrupt();
	return -1;
}

int main(int argc, char * argv[])
{
	Py_SetProgramName(argv[0]);  // should be called before Py_Initialize()
	Py_Initialize();

	cout << "start thread" << std::endl;
	std::thread loop_thread{thread_func};

	std::this_thread::sleep_for(std::chrono::seconds{3});

	cout << "interupting thread ..." << std::endl;
	PyGILState_STATE gil = PyGILState_Ensure();
	Py_AddPendingCall(quit, NULL);
	PyGILState_Release(gil);

	PyErr_SetInterrupt();

	loop_thread.join();

	Py_Finalize();

	cout << "done" << std::endl;
	return 0;
}
