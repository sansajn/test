// kate: replace-tabs off;

#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>

//! thread safe concurent queue implementation
template<typename T>
class concurrent_queue
{
public:
	concurrent_queue();
	concurrent_queue(concurrent_queue const & rhs);
	void push(T x);
	void wait_and_pop(T & result);

	template <typename Rep, typename Period>
	bool wait_and_pop(T & result, std::chrono::duration<Rep, Period> const & rel_time);

	std::shared_ptr<T> wait_and_pop();
	bool try_pop(T & result);
	std::shared_ptr<T> try_pop();
	bool empty() const;

private:
	using lock_guard = std::lock_guard<std::mutex>;

	std::queue<T> _data;
	mutable std::mutex _m;
	std::condition_variable _cond;
};


template<typename T>
concurrent_queue<T>::concurrent_queue()
{}

template<typename T>
concurrent_queue<T>::concurrent_queue(concurrent_queue const & rhs)
{
	lock_guard lk{rhs._m};
	_data = rhs._data;
}

template<typename T>
void concurrent_queue<T>::push(T x)
{
	lock_guard lk{_m};
	_data.push(x);
	_cond.notify_one();
}

template<typename T>
void concurrent_queue<T>::wait_and_pop(T & result)
{
	std::unique_lock<std::mutex> lk{_m};
	_cond.wait(lk, [this]{return !_data.empty();});
	result = _data.front();
	_data.pop();
}

template <typename T>
template <typename Rep, typename Period>
bool concurrent_queue<T>::wait_and_pop(T & result, std::chrono::duration<Rep, Period> const & rel_time)
{
	std::unique_lock<std::mutex> lk{_m};
	bool timeout = !_cond.wait_for(lk, rel_time, [this]{return !_data.empty();});
	if (timeout)
		return false;
	result = _data.front();
	_data.pop();
	return true;
}

template<typename T>
std::shared_ptr<T> concurrent_queue<T>::wait_and_pop()
{
	std::unique_lock<std::mutex> lk{_m};
	_cond.wait(lk, [this]{return !_data.empty();});
	std::shared_ptr<T> result{std::make_shared<T>(_data.front())};  // NOTE: only for types not throw an exception
	_data.pop();
	return result;
}

template<typename T>
bool concurrent_queue<T>::try_pop(T & result)
{
	lock_guard lk{_m};
	if (_data.empty())
		return false;
	result = _data.front();
	_data.pop();
	return true;
}

template<typename T>
std::shared_ptr<T> concurrent_queue<T>::try_pop()
{
	lock_guard lk{_m};
	if (_data.empty())
		return std::shared_ptr<T>{};
	std::shared_ptr<T> result{std::make_shared<T>(_data.front())};
	_data.pop();
	return result;
}

template<typename T>
bool concurrent_queue<T>::empty() const
{
	lock_guard lk{_m};
	return _data.empty();
}
