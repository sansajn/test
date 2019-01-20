// stack backtrace using gcc and linux api
#include <iostream>
#include <unwind.h>
#include <dlfcn.h>
#include <execinfo.h>  // backtrace

using std::cout;

using native_frame_ptr_t = void const *;

_Unwind_Reason_Code unwind_callback(_Unwind_Context * ctx, void * arg)
{
	_Unwind_Ptr ptr = _Unwind_GetIP(ctx);  // ziska hodnotu IP (Instruction Pointer) registra pre danny kontext
	cout << (void *)ptr << " -> ";

	// name
	Dl_info dli;
	int res = dladdr((void *)ptr, &dli);
	if (res != 0 && dli.dli_sname)
		cout << dli.dli_sname;

	// location
	if (res != 0 && dli.dli_fname)
		cout << " in " << dli.dli_fname;

	cout << "\n";

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
//	_Unwind_Backtrace(unwind_callback, nullptr);

	hoo h;
	h();

	return 0;
}
