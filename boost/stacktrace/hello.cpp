// boost stackstrace sample
#include <iostream>
#include <csignal>
#include <boost/stacktrace.hpp>
#include <string.h>  // for posix strsignal()

using std::cout, std::endl;

void verbose_signal_handler(int signal) {
	cout << "signal '" << strsignal(signal) << "' (" << signal << ") caught\n"
		<< "stacktrace:\n"
		<< boost::stacktrace::stacktrace{} 
		<< endl;

	exit(signal);
}

struct foo {
	void seg_fault() {
		raise(SIGSEGV);
	}
};

void goo() {
	foo f;
	f.seg_fault();
}

int main(int argc, char * argv[]) {
	signal(SIGSEGV, verbose_signal_handler);
	goo();
	cout << "done!\n";
	return 0;
}
