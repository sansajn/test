// strategized locking via compile-time polymorphism
#include <shared_mutex>
#include <mutex>
#include <iostream>

using std::cout;
using std::endl;
using std::mutex;
using std::shared_mutex;

template <typename LOCK>
class strategized_locking
{
public:
	strategized_locking(LOCK & l)
		: _lock{l}
	{
		_lock.lock();
	}

	~strategized_locking() {_lock.unlock();}

private:
	LOCK & _lock;
};

struct null_object_mutex
{
	void lock() {}
	void unlock() {}
};

class no_lock
{
public:
	void lock() const
	{
		cout << "no_lock::lock" << endl;
		_locker.lock();
	}

	void unlock() const
	{
		cout << "no_lock::unlock" << endl;
		_locker.unlock();
	}

private:
	mutable null_object_mutex _locker;
};

class exclusive_lock
{
public:
	void lock() const
	{
		cout << "   exclusive_lock::lock" << endl;
		_locker.lock();
	}

	void unlock() const
	{
		cout << "   exclusive_lock::unlock" << endl;
		_locker.unlock();
	}

private:
	mutable mutex _locker;
};

class shared_lock
{
public:
	void lock() const
	{
		cout << "      sharedlock::lock" << endl;
		_locker.lock_shared();
	}

	void unlock() const
	{
		cout << "      sharedlock::unlock" << endl;
		_locker.unlock_shared();
	}

private:
	mutable shared_mutex _locker;
};

int main(int argc, char * argv[])
{
	no_lock nl;
	strategized_locking strat_lock1{nl};

	{
		exclusive_lock el;
		strategized_locking strat_lock2{el};

		{
			shared_lock sl;
			strategized_locking strat_lock3{sl};
		}
	}

	return 0;
}
