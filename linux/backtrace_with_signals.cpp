// basically backtrace with fault address handling
#include <cstdlib>
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

void verbose_terminate_handler(int sig_num, siginfo_t * info, void * ctx)
{
	ucontext_t * uc = (ucontext_t *)ctx;

	// get the address at the time the signal was raised (gcc specific)
	void * caller_address;
#if defined(__i386__)
	caller_address = (void *)uc->uc_mcontext.gregs[REG_EIP];  // EIP: x86 specific
#elif defined(__x86_64__)
	caller_address = (void *)uc->uc_mcontext.gregs[REG_RIP];  // RIP: x86_64 specific
#else
	#error Unsupported architecture.
#endif

	fprintf(stderr, "signal %d (%s), address is %p from %p\n",
		sig_num, strsignal(sig_num), info->si_addr, (void *)caller_address);

	void * array[50];
	int size = backtrace(array, 50);

	array[1] = caller_address;  // overwrite sigaction with caller's address

	char ** messages = backtrace_symbols(array, size);

	// skip first stack frame (points here)
	for (int i = 1; i < size && messages != NULL; ++i)
		fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);

	free(messages);

	abort();
}

int crash()
{
	char * p = NULL;
	*p = 0;
	return 0;
}

int foo4()
{
	crash();
	return 0;
}

int foo3()
{
	foo4();
	return 0;
}

int foo2()
{
	foo3();
	return 0;
}

int foo1()
{
	foo2();
	return 0;
}

int main(int argc, char ** argv)
{
	struct sigaction sigact;

	sigact.sa_sigaction = verbose_terminate_handler;
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;

	if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
	{
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
				  SIGSEGV, strsignal(SIGSEGV));

		exit(EXIT_FAILURE);
	}

	foo1();

	exit(EXIT_SUCCESS);
}
