// idle sample
#include <iostream>
#include <glibmm/main.h>

using std::cout, std::endl;
using Glib::MainLoop, Glib::RefPtr, Glib::signal_idle;

bool idle_func() {
	cout << "hello!" << endl;
	return true;
}

int main(int argc, char * asrv[]) {
	cout << "press ctrl+c to quit\n";
	
	RefPtr<MainLoop> loop = MainLoop::create();

	sigc::slot<bool> idle_slot = sigc::ptr_fun(&idle_func);
	signal_idle().connect(idle_slot);

	loop->run();  // blocking
	
	cout << "done!\n";
	return 0;
}
