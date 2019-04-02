#include <string>
#include <iostream>
#include <cxxabi.h>
#include <signal.h>
#include <execinfo.h>
#include <ucontext.h>
#include <boost/asio.hpp>


using std::string;
using std::cout;
namespace asio = boost::asio;

void install_verbose_terminate_handler();
void verbose_sigsegv_terminate_handler(int sig_num, siginfo_t * info, void * ctx);
string demangle_message(char const * message);  //!< demangle "./test(crash__3foo+0x13) [0x8048e07]" like message
string demangle(string const & mangled_name);




int main(int argc, char * argv[])
{
	install_verbose_terminate_handler();
	
	asio::io_service io;
	
	cout << "press ctrl+c to exit";
	cout.flush();
	
	asio::signal_set sigint{io, SIGINT};
	sigint.async_wait([&io](boost::system::error_code const & ec, int signal_number) {
		io.stop();
	});
	
	io.run();
	cout << "done!" << std::endl;
	
	return 0;
}

void install_verbose_terminate_handler()
{
	struct sigaction sigact;

	sigact.sa_sigaction = verbose_sigsegv_terminate_handler;
	sigact.sa_flags = SA_RESTART|SA_SIGINFO;

	if (sigaction(SIGSEGV, &sigact, (struct sigaction *)nullptr) != 0)
	{
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
			SIGSEGV, strsignal(SIGSEGV));
	}

	if (sigaction(SIGABRT, &sigact, (struct sigaction *)nullptr) != 0)
	{
		fprintf(stderr, "error setting signal handler for %d (%s)\n",
				SIGABRT, strsignal(SIGABRT));
	}
}

void verbose_sigsegv_terminate_handler(int sig_num, siginfo_t * info, void * ctx)
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

	for (int i = 1; i < size && messages != NULL; ++i)  // skip first stack frame (points here)
	{
		string demangled = demangle_message(messages[i]);
		fprintf(stderr, "[bt]: (%d) %s\n", i, demangled.c_str());
	}

	free(messages);

	//abort();  // we want abort() there so system can create core dump after crash
	raise(SIGKILL); //Don't use abort here otherwise we have infinite loop because we handle SIGABRT signal
}

string demangle_message(char const * message)
{
	string s = message;
	size_t beg = s.find("(");
	size_t end = s.rfind("+");
	if (beg < end && beg != string::npos && end != string::npos)
	{
		string mangled_name = s.substr(beg+1, end - beg - 1);
		string demangled_name = demangle(mangled_name);
		s.replace(beg+1, end - beg - 1, demangled_name);
	}
	return s;
}

string demangle(string const & mangled_name)
{
	int status = 0;
	char * real_name = abi::__cxa_demangle(mangled_name.c_str(), nullptr, 0, &status);

	string result;
	if (status == 0)
		result = real_name;
	else
		result = mangled_name;

	return result;
}
