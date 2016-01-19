/* how to use the backtrace functions (listing 1. s clanku http://www.linuxjournal.com/article/6391?page=0,0) 
skompiluj prikzom 'g++ -rdynamic backtrace.cpp -o backtrace' */ 
#include <cstdio>
#include <signal.h>
#include <execinfo.h>

void show_stackframe() 
{
	void * trace[16];
	int trace_size = backtrace(trace, 16);
	char ** messages = backtrace_symbols(trace, trace_size);
	printf("[bt] Execution path:\n");
	for (int i = 0; i < trace_size; ++i)
		printf("[bt] %s\n", messages[i]);
}

int func_low(int p1, int p2)
{
	p1 = p1 - p2;
	show_stackframe();
	return 2*p1;
}

int func_hight(int p1, int p2)
{
	p1 = p1 + p2;
	show_stackframe();
	return 2*p1;
}

int test(int p1)
{
	int res;
	if (p1 < 10)
		res = 5 + func_low(p1, 2*p1);
	else
		res = 5 + func_hight(p1, 2*p1);
	return res;
}

int main(int argc, char * argv[])
{
	printf("First call: %d\n\n", test(27));
	printf("Second call: %d\n", test(4));
	return 0;
}
