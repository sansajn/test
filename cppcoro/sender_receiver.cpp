// sender receiver pattern with coroutines (compare with `condition_variables.cpp`) sample

#include <coroutine>
#include <chrono>
#include <iostream>
#include <functional>
#include <string>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <boost/noncopyable.hpp>

using std::thread;
namespace this_thread = std::this_thread;
using std::coroutine_handle, std::suspend_never;
using std::atomic;
using std::runtime_error;
using std::chrono::high_resolution_clock, std::chrono::duration;
using namespace std::chrono_literals;
using std::cout, std::endl;

class Event : private boost::noncopyable
{
public:
	class Awaiter;
	Awaiter operator co_await() const noexcept;

	void notify() noexcept;

private:
	friend class Awaiter;

	mutable atomic<bool> notified{false};
	mutable atomic<void *> suspendedWaiter{nullptr};
};

class Event::Awaiter
{
public:
	Awaiter(Event const & e) : _event{e} {}
	bool await_ready() const;
	bool await_suspend(coroutine_handle<> handler) noexcept;
	void await_resume() noexcept {}

private:
	friend class Event;

	Event const & _event;
	coroutine_handle<> _handler;
};

bool Event::Awaiter::await_ready() const
{
	// allow at most one waiter
	if (_event.suspendedWaiter.load() != nullptr)
		throw runtime_error{"more than one waiter is not valid"};
	
	// _event.notified == false; suspends the coroutine
   // _event.notified == true; the coroutine is executed such as a usual function
	return _event.notified;
}

bool Event::Awaiter::await_suspend(coroutine_handle<> handler) noexcept
{
	_handler = handler;

	if (_event.notified)	
		return false;

	_event.suspendedWaiter.store(this);
	
	return true;
}

void Event::notify() noexcept
{
	notified = true;

	auto * waiter = static_cast<Awaiter *>(suspendedWaiter.load());
	if (waiter)
		waiter->_handler.resume();  // resume the coroutine => await_resume
}

Event::Awaiter Event::operator co_await() const noexcept
{
	return Awaiter{*this};
}

struct Task 
{
	struct promise_type
	{
		auto get_return_object() {return Task{};}
		auto initial_suspend() {return suspend_never{};}
		auto final_suspend() {return suspend_never{};}
		void return_void() {}
		void unhandled_exception() {}
	};
};

Task receiver(Event & event)
{
	auto start = high_resolution_clock::now();
	co_await event;
	cout << "got the notification!" << endl;
	duration<double> elapsed = high_resolution_clock::now() - start; 
	cout << "waited " << elapsed.count() << "s" << endl;
}

int main(int argc, char * argv[])
{
	cout << "notificatoin before waiting" << endl;

	Event event1;
	thread sender_thread{[&event1]{event1.notify();}},
		receiver_thread{receiver, std::ref(event1)};

	sender_thread.join();
	receiver_thread.join();

	cout << "\n"
		<< "notification after 2 seconds waiting" << endl;

	Event event2;
	thread receiver_thread2{receiver, std::ref(event2)};
	thread sender_thread2{[&event2]{
		this_thread::sleep_for(2s);
		event2.notify();
	}};

	receiver_thread2.join();
	sender_thread2.join();

	cout << endl;

	return 0;
}
