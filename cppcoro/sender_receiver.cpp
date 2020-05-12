// sender receiver pattern with coroutines (compare with `condition_variables.cpp`) sample

#include <coroutine>
#include <chrono>
#include <iostream>
#include <functional>
#include <string>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <boost/noncopyable.hpp

using std::coroutine_handle;
using std::atomic;

class Event : private boost::noncopyable
{
public:
	class Awaiter;
	Awaiter operator co_wait() const noexcept;

	void notify() noexcept;

private:
	friend class Awaiter;

	mutable atomic<bool> notified{false};
	mutable atomic<void *> suspendWaiter{nullptr};
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
	if (_event.suspendWaiter.load() != nullptr)
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

	_event.suspendWaiter.store(this);
}
