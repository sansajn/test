#pragma once
#include <iostream>


namespace test {
	namespace sub {

template <typename T>
void printer(T const & t);

template <>
void printer<int>(int const & t)
{
	std::cout << "printer<int>(int const &)\n";
}

class foo
{
public:
	template <typename T>
	foo & operator<<(T const & rhs) {
		printer<T>(rhs);
	}
};

	}  // sub
}  // test

