// usage of promise and future

#include <future>
#include <iostream>
#include <thread>
#include <utility>

using namespace std;

void product(promise<int> && p, int a, int b)
{
	p.set_value(a*b);
}

struct div_functor
{
	void operator()(promise<int> && p, int a, int b) const
	{
		p.set_value(a/b);
	}
};

int main(int argc, char * argv[])
{
	int a = 20, b = 10;

	// define promises
	promise<int> prod_promise, div_promise;

	// define futures
	future<int> prod_result = prod_promise.get_future(),
		div_result = div_promise.get_future();

	// calculate result in a separate thread
	thread prod_thread{product, move(prod_promise), a, b};
	thread div_thread{div_functor{}, move(div_promise), a, b};

	// get the result
	cout << "20*10=" << prod_result.get() << "\n"
		<< "20/10=" << div_result.get() << endl;

	prod_thread.join();
	div_thread.join();

	return 0;
}
