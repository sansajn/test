// strategized locking via runtime polymorfism (strategy pattern sample)
#include <mutex>
#include <shared_mutex>
#include <iostream>

using std::cout;
using std::endl;
using std::mutex;
using std::shared_mutex;

class base_lock
{
public:
	virtual void lock() const = 0;
	virtual void unlock() const = 0;
};

class strategized_locking
{
public:
	strategized_locking(base_lock & l)
		: _lock{l}
	{
		_lock.lock();
	}

	~strategized_locking() {_lock.unlock();}

private:
	base_lock & _lock;
};

struct null_object_mutex
{
	void lock() {}
	void unlock() {}
};

class no_lock : public base_lock
{
	void lock() const override
	{
		cout << "no_lock::lock" << endl;
		_locker.lock();
	}

	void unlock() const override
	{
		cout << "no_lock::unlock" << endl;
		_locker.unlock();
	}

	mutable null_object_mutex _locker;
};

class exclusive_lock : public base_lock
{
	void lock() const override
	{
		cout << "   exclusive_lock::lock" << endl;
		_locker.lock();
	}

	void unlock() const override
	{
		cout << "   exclusive_lock::unlock" << endl;
		_locker.unlock();
	}

	mutable mutex _locker;
};

class shared_lock : public base_lock
{
	void lock() const override
	{
		cout << "      sharedlock::lock" << endl;
		_locker.lock_shared();
	}

	void unlock() const override
	{
		cout << "      sharedlock::unlock" << endl;
		_locker.unlock_shared();
	}

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
