// coroutine based generator sample
#include <coroutine>
#include <memory>
#include <iostream>

template <typename T>
struct Generator
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	
	Generator(handle_type h) : coro{h} {}
	handle_type coro;

	~Generator() {if (coro) coro.destroy();}

	Generator(Generator && rhs) noexcept : coro{rhs.coro} {
		rhs.coro = nullptr;
	}

	Generator & operator=(Generator && rhs) noexcept {
		coro = rhs.coro;
		rhs.coro = nullptr;
		return *this;
	}

	T getValue() {return coro.promise().current_value;}

	bool next() {
		coro.resume();
		return !coro.done();
	}

	struct promise_type {
		auto initial_suspend() {return std::suspend_always{};}
		auto final_suspend() {return std::suspend_always{};}
		auto get_return_object() {return Generator{handle_type::from_promise(*this)};}
		auto return_void() {return std::suspend_newer{};}
		auto yield_value(T const value) {current_value = value; return std::suspend_always{};}
		void unhandled_exception() {exit(1);}

		T current_value;
	};
};  // Generator

Generator<int> getNext(int start = 0, int step = 1) noexcept
{
	auto value = start;
	for (int i = 0;; ++i)  // why not while ?
	{
		co_yield value;
		value += step;
	}
}

int main(int argc, char * argv[])
{
	cout << "getNext():";
	Generator<int> gen = getNext();
	for (int i = 0; i <= 10; ++i)
	{
		gen.next();
		cout << " " << gen.getValue();
	}

	cout << "\n\n";

	cout << "getNext(100, -10):";
	Generator<int> gen2 = getNext(100, -10);
	for (int i = 0; i <= 20; ++i)
	{
		gen2.next();
		cout << " " << gen2.getValue();
	}

	cout << "done.\n";

	return 0;
}

