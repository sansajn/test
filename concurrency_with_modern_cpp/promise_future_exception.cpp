// returning an exception
#include <exception>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

using namespace std::literals::string_literals;
using namespace std;

struct div_operation
{
	void operator()(promise<int> && p, int a, int b)
	{
		try {
			if (b == 0)
				throw runtime_error{"Illegal division by zero "s + to_string(a) + "/" + to_string(b)};

			p.set_value(a/b);
		}
		catch (...) {
			p.set_exception(current_exception());
		}
	}
};

void execute_division(int nom, int denom)
{
	promise<int> div_promise;
	future<int> div_result = div_promise.get_future();

	thread div_thread{div_operation{}, move(div_promise), nom, denom};

	// get the result or the exception
	try {
		cout << nom << "/" << denom << "=" << div_result.get() << endl;
	}
	catch (runtime_error & e) {
		cout << e.what() << endl;
	}

	div_thread.join();
}

int main(int argc, char * argv[])
{
	execute_division(20, 0);
	execute_division(20, 10);

	return 0;
}
