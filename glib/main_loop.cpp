// GMainLoop usage sample
#include <iostream>
#include <glib.h>

using std::cout, std::endl;

gboolean say_hello(void *) {
	cout << "hello!" << endl;
	return TRUE;
}

int main(int argc, char * argv[]) {
	cout << "press ctrl+c to quit\n";
	GMainLoop * loop = g_main_loop_new(NULL, FALSE);
	g_timeout_add_seconds(1, (GSourceFunc)say_hello, nullptr);  // the function use global-default context (will not work with thread-default context)

	g_main_loop_run(loop);  // blocking
	
	g_main_loop_unref(loop);
	return 0;
}
