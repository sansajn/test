/*! system handle challange 21 from *The Modern C++ Challenge* book */
#include <iostream>
#include <cstdio>
using std::cout;

//! safe system handle wrapper
template <typename Traits>
class unique_handle {
	using pointer = typename Traits::pointer;

public:
	unique_handle(unique_handle const &) = delete;
	unique_handle & operator=(unique_handle const &) = delete;

	explicit unique_handle(pointer value = Traits::invalid()) noexcept
		: _value{value}
	{}

	unique_handle(unique_handle && other) noexcept
		: _value{other.release()}
	{}

	unique_handle & operator=(unique_handle && other) noexcept {
		if (this != &other)
			reset(other.release());
		return *this;
	}

	~unique_handle() noexcept {
		Traits::close(_value);
	}

	explicit operator bool() const noexcept {
		return _value != Traits::invalid();
	}

	pointer get() const noexcept {
		return _value;
	}

	pointer release() noexcept	{
		auto value = _value;
		_value = Traits::invalid();
		return value;
	}

	bool reset(pointer value = Traits::invalid()) noexcept {
		if (_value != value) {
			Traits::close(_value);
			_value = value;
		}
		return static_cast<bool>(*this);
	}

	void swap(unique_handle<Traits> & other) noexcept {
		std::swap(_value, other._value);
	}

private:
	pointer _value;
};

template <typename Traits>
void swap(unique_handle<Traits> & left, unique_handle<Traits> & right) {
	left.swap(right);
}

template <typename Traits>
bool operator==(unique_handle<Traits> const & left, unique_handle<Traits> const & right) noexcept {
	return left.get() == right.get();
}

template <typename Traits>
bool operator!=(unique_handle<Traits> const & left, unique_handle<Traits> const & right) noexcept {
	return left.get() != right.get();
}

struct file_handle_traits {
	using pointer = FILE *;
	static pointer invalid() noexcept {return nullptr;}
	static void close(pointer value) noexcept {fclose(value);}
};

using file_handle = unique_handle<file_handle_traits>;


int main(int argc, char * argv[]) {
	file_handle fin{fopen("system_handle.cpp", "r")};
	if (!fin)
		return 1;

	// read file
	char buf[4096];
	size_t bytes = fread(buf, sizeof(char), 4096, fin.get());
	cout << "readed "<< bytes << " bytes\n";

	cout << "done!\n";
	return 0;
}