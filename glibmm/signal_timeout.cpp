// signal_timeout sample
#include <iostream>
#include <glibmm/main.h>

using std::cout, std::endl;
using Glib::MainLoop, Glib::RefPtr, Glib::signal_timeout;

bool say_hello() {
	cout << "hello!" << endl;
	return true;
}

int main(int argc, char * asrv[]) {
	cout << "press ctrl+c to quit\n";
	
	RefPtr<MainLoop> loop = MainLoop::create();

	sigc::slot<bool> hello_slot = sigc::ptr_fun(&say_hello);
	signal_timeout().connect_seconds(hello_slot, 1);  // call hello every 1s

	loop->run();  // blocking
	
	cout << "done!\n";
	return 0;
}
