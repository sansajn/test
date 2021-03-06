// binding promise in async like function sample
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <iostream>
using std::promise, std::future;
using std::thread;
using std::move;
using std::cout, std::endl;

int meaning_of_life() {return 42;}

template<typename F>
auto foo(F && f) -> future<decltype(f())>
{
	using result_type = decltype(f());
	promise<result_type> p;
	future<result_type> result = p.get_future();

	thread{[result = move(p), fce = move(f)]() mutable {
		try {
			result.set_value(fce());
		}
		catch (...) {
			result.set_exception(std::current_exception());
		}
	}}.detach();

	return result;
}

int main(int argc, char * argv[])
{
	auto answer = foo([]{return meaning_of_life();});
	cout << "the answer is " << answer.get() << "\n";

	cout << "done\n";
	return 0;
}
