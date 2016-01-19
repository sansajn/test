// chitanie signalou pri pade aplykacie
#include <iostream>
#include <execinfo.h>
// posix
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
