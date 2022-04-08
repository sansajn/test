// MainLoop sample
#include <iostream>
#include <glibmm/main.h>

using std::cout, std::endl;
using Glib::MainLoop, Glib::RefPtr;

gboolean say_hello(void *) {
	cout << "hello!" << endl;
	return TRUE;
}

int main(int argc, char * asrv[]) {
	cout << "press ctrl+c to quit\n";
	
	RefPtr<MainLoop> loop = MainLoop::create();
	
	g_timeout_add_seconds(1, (GSourceFunc)say_hello, nullptr);  // the function use global-default context (will not work with thread-default context)

	loop->run();  // blocking
	
	cout << "done!\n";
	return 0;
}
