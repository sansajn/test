#pragma once
#include <thread>

//! Joinable thread implementation for testing purpose.
class jthread : public std::thread {
public:
	using thread::thread;  // reuse constructors

	~jthread() {
		if (joinable())
			join();
	}
};
