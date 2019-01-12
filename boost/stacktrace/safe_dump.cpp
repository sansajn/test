// safe_dump_to() sample
#include <fstream>
#include <iostream>
#include <exception>
#include <signal.h>
#include <boost/stacktrace.hpp>
#include <boost/stacktrace/safe_dump_to.hpp>
#include <boost/filesystem.hpp>

using std::cin;
using std::cout;
using std::ifstream;
using std::terminate;

void verbose_signal_handler(int signum)
{
	signal(signum, SIG_DFL);
	boost::stacktrace::safe_dump_to("./backtrace.dump");
	raise(SIGABRT);
}

void foo()
{
//	int * __faulty = nullptr;
//	*__faulty = 15;
	terminate();
}

void goo()
{
	foo();
}

int main(int argc, char * argv[])
{
	signal(SIGSEGV, &verbose_signal_handler);
	signal(SIGABRT, &verbose_signal_handler);

	if (boost::filesystem::exists("./backtrace.dump"))
	{
		ifstream fin{"./backtrace.dump"};
		boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(fin);
		cout << st << std::endl;
		fin.close();
		remove("./backtrace.dump");
	}
	else
		goo();

	cin.get();

	return 0;
}
