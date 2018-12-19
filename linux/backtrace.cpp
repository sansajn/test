/* how to use the backtrace functions (listing 1. s clanku http://www.linuxjournal.com/article/6391?page=0,0) 
skompiluj prikzom 'g++ -rdynamic backtrace.cpp -o backtrace'

$ ./backtrace
[bt] Execution path:
[bt] ./backtrace(_Z15show_stackframev+0x2e) [0x4009c4]
[bt] ./backtrace(_Z10func_hightii+0x19) [0x400a92]
[bt] ./backtrace(_Z4testi+0x3d) [0x400ad6]
[bt] ./backtrace(main+0x19) [0x400afa]
[bt] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0) [0x7f4ac67bb830]
[bt] ./backtrace(_start+0x29) [0x4008c9]
First call: 167

[bt] Execution path:
[bt] ./backtrace(_Z15show_stackframev+0x2e) [0x4009c4]
[bt] ./backtrace(_Z8func_lowii+0x19) [0x400a72]
[bt] ./backtrace(_Z4testi+0x23) [0x400abc]
[bt] ./backtrace(main+0x34) [0x400b15]
[bt] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0) [0x7f4ac67bb830]
[bt] ./backtrace(_start+0x29) [0x4008c9]
Second call: -3 */
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
