/* the boost way doing (stack) backtrace */
#include <string>
#include <memory>
#include <iostream>
#include <cstring>
#include <unwind.h>
#include <backtrace.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <unistd.h>

using std::string;
using std::to_string;
using std::unique_ptr;
using std::cout;

struct pc_data
{
	string * filename;
	int lineno;
	string * function;
};

int callback(void * data, uintptr_t /*pc*/, char const * filename, int lineno,
	char const * function)
{
	pc_data & d = *(pc_data *)data;
	if (d.filename && filename)
		*d.filename = filename;

	d.lineno = lineno;

	if (d.function && function)
		*d.function = function;

	return 0;
}

void error_callback(void * /*data*/, char const * /*msg*/, int /*errnum*/)
{}

string program_location()
{
	for (ssize_t max_path = 64; ; max_path *= 2)
	{
		unique_ptr<char []> buf{new char[max_path]};
		ssize_t bytes = readlink("/proc/self/exe", buf.get(), max_path);
		if (bytes == -1)
			throw std::runtime_error{"unable to found program location"};
		if (bytes != max_path)
			return string{buf.get()};
	}
}

string to_string_backtrace(void const * addr)
{
	// function name
	backtrace_state * state = backtrace_create_state(
		program_location().c_str(), 0, error_callback, nullptr);

	string filename, function;
	pc_data data = {&filename, 0, &function};
	backtrace_pcinfo(state, (uintptr_t)addr, callback, error_callback, &data);

	Dl_info dli;
	memset(&dli, 0, sizeof(Dl_info));
	if (function.empty())  // libbacktrace not working, try dl
	{
		if (dladdr(addr, &dli) != 0 && dli.dli_sname)
			function = dli.dli_sname;
	}

	// demangle
	int status = 0;
	size_t size = 0;
	char * demagled_function = abi::__cxa_demangle(function.c_str(), nullptr, &size, &status);

	string res{demagled_function ? demagled_function : function};

	free(demagled_function);

	// source location
	if (!filename.empty() && data.lineno != 0)
		return res += " at " + filename + ":" + to_string(data.lineno);

	// location from symbol (in a case source is unknown)
	if (dli.dli_fname)
		res += " in " + string{dli.dli_fname};

	return res;
}

_Unwind_Reason_Code unwind_callback(_Unwind_Context * ctx, void * arg)
{
	_Unwind_Ptr addr = _Unwind_GetIP(ctx);  // ziska hodnotu IP (Instruction Pointer) registra pre danny kontext
	cout << (void *)addr << " -> " << to_string_backtrace((void *)addr) << "\n";

	return _URC_NO_REASON;
}

void foo()
{
	_Unwind_Backtrace(unwind_callback, nullptr);
}

void goo()
{
	foo();
}

struct hoo
{
	void operator()() const {goo();}
};

int main(int argc, char * argv[])
{
	hoo h;
	h();
	return 0;
}
