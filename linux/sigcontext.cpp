// using sigcontext
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <execinfo.h>

void bt_sighandler(int sig, struct sigcontext ctx)
{
	if (sig == SIGSEGV)
		printf("got signal %d, faulty address is %p, from %p\n", sig, ctx.cr2, ctx.eip);
	else
		printf("got signal %d\n", sig);
	
	void * trace[16];
	int trace_size = backtrace(trace, 16);
	// overwrite sifaction with caller's address
	trace[1] = (void *)ctx.eip;
	char ** messages = backtrace_symbols(trace, trace_size);
	// skip first stack frame (points there)
	printf("[bt] Execution path:\n");
	for (int i =0; i < trace_size; ++i)
		printf("[bt] %s\n", messages[i]);
	exit(0);
}

int func_a(int a, char b)
{
	char * p = (char *)0xdeadbeef;
	a = a + b;
	*p = 10;  // crash
	return 2*a;
}

int func_b()
{
	int res, a = 5;
	res = 5 + func_a(a, 't');
	return res;
}

int main()
{
	struct sigaction sa;
	sa.sa_handler = bt_sighandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGUSR1, &sa, NULL);
	
	printf("%d\n", func_b());
}
