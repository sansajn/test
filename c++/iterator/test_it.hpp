#pragma once
#include <utility>
#include <cstddef>

template <typename It>
bool input_iterator_api_available() {
	It it1, it2;
	*it1;  // access value
	it1->first;  // access member variable
	++it1;  // pre increment
	it1++;  // post increment
	it1 == it2;  // equal operator
	it1 != it2;  // not equal operator
	It it3{it1};  // copy constructor
	return true;
}

template <typename It>
struct input_iterator_api_implemented {
	bool creation() {
		REQUIRE((*pos1 == std::pair<size_t, size_t>{0,0}));
		REQUIRE(pos1->first == 0);
		return true;
	}

	bool pre_increment() {
		++pos1;
		REQUIRE((*pos1 == std::pair<size_t, size_t>{1,0}));
		++pos1;
		++pos1;
		REQUIRE((*pos1 == std::pair<size_t, size_t>{1,1}));
		REQUIRE((*(++pos1) == std::pair<size_t, size_t>{0,2}));
		return true;
	}

	bool post_increment() {
		pos1++;
		REQUIRE((*pos1 == std::pair<size_t, size_t>{1,0}));
		pos1++;
		pos1++;
		REQUIRE((*pos1 == std::pair<size_t, size_t>{1,1}));
		return true;
	}

	bool equal() {
		It pos2, pos3;
		REQUIRE(pos2 == pos3);

		It pos4{1,1};
		++pos4;
		++pos4;
		++pos4;  // (1,1)
		++pos4;  // {}
		REQUIRE(pos4 == It{});
		return true;
	}

	bool not_equal() {
		It pos2;
		REQUIRE(pos1 != pos2);
		return true;
	}

	bool copy_ctor() {
		++pos1; ++pos1; ++pos1;
		It pos2{pos1};
		REQUIRE((*pos2 == std::pair<size_t, size_t>{1,1}));
		return true;
	}

	It pos1{2,3};
};  // input_iterator_api_implemented
