#pragma once
#include <utility>
#include <cstddef>
#include <catch.hpp>

template <typename Iter>
bool input_iterator_api_available() {
	Iter it1{3,3};
	*it1;  // access value
	it1->first;  // access member variable
	++it1;  // pre increment
	it1++;  // post increment
	Iter it2{1,2};
	it1 == it2;  // equal operator
	it1 != it2;  // not equal operator
	Iter it3{it1};  // copy constructor
	return true;
}

template <typename Iter>
bool forward_iterator_api_available() {
	REQUIRE(input_iterator_api_available<Iter>());
	Iter it1, it2;
	it1 = it2;  // assign operator
	return true;
}

template <typename Iter>
bool bidirectional_iterator_api_available() {
	REQUIRE(forward_iterator_api_available<Iter>());
	Iter pos4{2,2};
	++pos4;
	++pos4;
	--pos4;  // pre decrement
	pos4--;  // post decrement
	return true;
}

template <typename Iter>
bool random_access_iterator_api_available() {
	REQUIRE(bidirectional_iterator_api_available<Iter>());
	Iter it5{5,5};
	it5[3];  // n-th element access
	it5 += 3;  // step n elements forward
	it5 - 3;  // nth previous element
	it5 -= 3;  // step n elements backward
	it5 + 3;  // nth next element
	3 + it5;
	Iter it6 = it5+3;
	it6 - it5;  // distance
	it5 < it6;  // pos5 before pos6
	it5 > it6;  // pos5 after pos6
	it5 <= it6;  // pos5 not after pos6
	it5 >= it6;  // pos5 not before pos6
	return true;
}

template <typename Iter>
struct input_iterator_api_implemented {  // rename to _implementation
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

	Iter pos1{2,3};  // TODO: remove it (mutable state)
};  // input_iterator_api_implemented

template <typename Iter>
struct forward_iterator_api_implemented : public input_iterator_api_implemented<Iter> {
	using input_iter = input_iterator_api_implemented<Iter>;

	bool creation() {
		REQUIRE(input_iter::creation());  // check input iterator implementation
		Iter pos2;
		REQUIRE((*pos2 == std::pair<size_t, size_t>{0,0}));
		return true;
	}

	bool post_increment() {
		REQUIRE(input_iter::post_increment());
		REQUIRE((*(this->pos1++) == std::pair<size_t, size_t>{1,1}));
		return true;
	}

	bool equal() {
		REQUIRE(input_iter::equal());
		Iter pos2{2, 3};
		REQUIRE(this->pos1 == pos2);
		return true;
	}

	bool assign() {
		Iter pos2;
		REQUIRE(this->pos1 != pos2);
		pos2 = this->pos1;
		REQUIRE(this->pos1 == pos2);
		++this->pos1;
		REQUIRE(this->pos1 != pos2);
		++pos2;
		REQUIRE(this->pos1 == pos2);
		return true;
	}
};

template <typename Iter>
struct bidirectional_iterator_api_iplemented : public forward_iterator_api_implemented<Iter> {
	using forward_iter = forward_iterator_api_implemented<Iter>;

	bool pre_decrement() {
		++this->pos1;
		++this->pos1;
		++this->pos1;
		++this->pos1;
		++this->pos1;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{1,2}));
		--this->pos1;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{0,2}));
		--this->pos1;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{1,1}));
		REQUIRE((*(--this->pos1) == std::pair<size_t, size_t>{0,1}));
		return true;
	}

	bool post_decrement() {
		++this->pos1;
		++this->pos1;
		++this->pos1;
		++this->pos1;
		++this->pos1;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{1,2}));
		this->pos1--;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{0,2}));
		this->pos1--;
		REQUIRE((*this->pos1 == std::pair<size_t, size_t>{1,1}));
		REQUIRE((*(this->pos1--) == std::pair<size_t, size_t>{1,1}));
		return true;
	}
};

