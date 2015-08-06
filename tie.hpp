/*! Implementacia tie funkcie pre dvojicu.
\author Adam Hlavatovic
\version 20110831 */
#pragma once

// std
#include <utility>


namespace loe {

	namespace detail {


template <typename T, typename U>
struct tier
{
	T & first;
	U & second;

	tier(T & f, U & s)
		: first(f), second(s)
	{}

	template <typename R, typename S>
	tier<T, U> & operator=(const std::pair<R, S> & p)
	{
		first = p.first;
		second = p.second;
		return *this;
	}
};

}  // detail


/*! Umožnuje nasledujuci zápis
\code
	int a, b;
	tie(a, b) = std::make_pair(1, 2);
	... */
template <typename T, typename U>
inline detail::tier<T, U> tie(T & a, U & b)
{
	return detail::tier<T, U>(a, b);
}

}  // loe

