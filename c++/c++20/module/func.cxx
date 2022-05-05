// simple C++ module sample
module;

#include <iostream>

export module mod;

export void func() {
	std::cout << "hello, world from module!\n";
}
