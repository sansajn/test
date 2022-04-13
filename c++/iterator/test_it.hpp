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
bool forward_iterator_api_available() {
	It it1;  // default constructor
	*it1;  // access position as (x,y) pair
	it1->first;  // access x
	++it1;  // pre increment
	it1++;  // post increment
	It it2;
	it1 == it2;  // equal operator
	it1 != it2;  // not equal operator
	It it3{it1};  // copy constructor
	it1 = it2;  // assign operator
	return true;
}

template <typename Iter>
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
		Iter pos2{2, 3};
		REQUIRE(pos1 == pos2);

		Iter pos3, pos4;
		REQUIRE(pos3 == pos4);

		Iter pos5{1,1};
		++pos5;
		++pos5;
		++pos5;  // (1,1)
		++pos5;  // {}
		REQUIRE(pos5 == Iter{});
		return true;
	}

	bool not_equal() {
		Iter pos2;
		REQUIRE(pos1 != pos2);
		return true;
	}

	bool copy_ctor() {
		++pos1; ++pos1; ++pos1;
		Iter pos2{pos1};
		REQUIRE((*pos2 == std::pair<size_t, size_t>{1,1}));
		return true;
	}

	Iter pos1{2,3};
};  // input_iterator_api_implemented
