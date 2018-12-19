/* chitanie signalou pri pade aplykacie a generovanie stack-trace

$ ./sighandler
./sighandler(_Z17print_stack_tracev+0x2e)[0x400d24]
./sighandler(_Z14custom_handleri+0x10)[0x400d73]
/lib/x86_64-linux-gnu/libc.so.6(+0x354b0)[0x7f63e88e54b0]
./sighandler(_ZN3foo4funcEv+0x11)[0x400e25]
./sighandler(_Z11fault_functv+0x1c)[0x400d94]
./sighandler(main+0x23)[0x400dba]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0)[0x7f63e88d0830]
./sighandler(_start+0x29)[0x400c29]
Aborted (core dumped) */
#include <iostream>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

void print_stack_trace()
{
	void * array[30];
	size_t size = backtrace(array, 30);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
}

void custom_handler(int signum)
{
	print_stack_trace();
	std::abort();
}

struct foo 
{
	void func() {
		std::cout << "foo::func():{n=}" << _n << std::endl;
	}
	
	int _n;
};

void fault_funct()
{
	foo * f = nullptr;
	f->func();  // Segmentation fault (core dumped)
}

int main(int argc,char * argv[])
{
	signal(SIGSEGV, custom_handler);
	fault_funct();
	return 0;
}
