/* stack trace on SIGSEGV signal sample

$ ./stack_trace_on_sigsegv
Error: signal 11:
./stack_trace_on_sigsegv(_Z7handleri+0x2b)[0x400af1]
/lib/x86_64-linux-gnu/libc.so.6(+0x354b0)[0x7f5a2f0f34b0]
./stack_trace_on_sigsegv(_Z3bazv+0x14)[0x400b79]
./stack_trace_on_sigsegv(_Z3barv+0x9)[0x400b98]
./stack_trace_on_sigsegv(_Z3foov+0x9)[0x400ba4]
./stack_trace_on_sigsegv(main+0x23)[0x400bca]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0)[0x7f5a2f0de830]
./stack_trace_on_sigsegv(_start+0x29)[0x4009f9]
Segmentation fault (core dumped) */

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig)
{
	void *array[10];

	// get void*'s for all entries on the stack
	size_t size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);

	// resend SIGSEGV to get core dump
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

void baz()
{
	int *foo = (int*)-1; // make a bad pointer
	printf("%d\n", *foo); // causes segfault
}

void bar() { baz(); }
void foo() { bar(); }


int main(int argc, char * argv[])
{
	signal(SIGSEGV, handler);   // install our handler
	foo(); // this will call foo, bar, and baz.  baz segfaults.
}
