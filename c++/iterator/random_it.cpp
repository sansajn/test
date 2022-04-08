// Random access iterator implementation sample.
#include <algorithm>
#include <utility>
#include <iterator>
#include <filesystem>
#include <execution>
#include <cmath>
#include <cstdint>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "image.hpp"
using std::transform, std::pair, std::begin, std::end, std::filesystem::path;

path const gradient_image = "random_gradient.png";

//! Random access iterator view implemenation.
struct pixel_pos_view
	: public std::iterator<std::random_access_iterator_tag, pair<size_t, size_t>> {

	pixel_pos_view() : pixel_pos_view{0, 0} {}

	pixel_pos_view(size_t w, size_t h)
		: _w{w}, _h{h}, _pos{0, 0}
	{}

	reference operator*() {
		return _pos;
	}

	pointer operator->() {
		return &_pos;
	}

	pixel_pos_view & operator++() {
		auto & [c, r] = _pos;
		c += 1;
		if (c >= _w) {
			c = 0;

			r += 1;
			if (r >= _h)
				r = _h;  // end of range _pos=(0, _h)
		}
		return *this;
	}

	pixel_pos_view operator++(int) {
		auto previous = *this;
		++(*this);
		return previous;
	}

	pixel_pos_view & operator--() {
		assert(_w > 0 && _h > 0);
		assert(_h != _pos.second);  // end of range iterator

		auto & [c, r] = _pos;
		if (c == 0 && r == 0) {
			r = _h;  // end of range _pos=(0, _h)
			return *this;
		}
		else {
			if (c == 0) {
				r -= 1;  // it is safe, r > 0 by defnition there
				c = _w-1;
			}
			else
				c -= 1;
		}
		return *this;
	}

	pixel_pos_view operator--(int) {
		auto previous = *this;
		--(*this);
		return previous;
	}

	value_type operator[](int n) {  //!< element access is not possibel, we can only return its value
		return {};  // TODO: implement
	}

	pixel_pos_view & operator+=(int n) {
		return *this;  // TODO: implement
	}

	pixel_pos_view & operator-=(int n) {
		return *this;  // TODO: implement
	}

	pixel_pos_view operator+(int n) const {
		return {};  // TODO: implement
	}

	pixel_pos_view operator-(int n) const {
		return {};  // TODO: implement
	}

	difference_type operator-(pixel_pos_view const & pos) {
		return 0;  // TODO: implement
	}

	bool operator<(pixel_pos_view const & pos) {
		return false;  // TODO: implement
	}

	bool operator>(pixel_pos_view const & pos) {
		return false;  // TODO: implement
	}

	bool operator<=(pixel_pos_view const & pos) {
		return false;  // TODO: implement
	}

	bool operator>=(pixel_pos_view const & pos) {
		return false;  // TODO: implement
	}

	bool operator==(pixel_pos_view const & rhs) const {
		return (_w == rhs._w && _h == rhs._h && _pos == rhs._pos)
			|| (_h == _pos.second && rhs._h == rhs._pos.second);  // end-iterator
	}

	bool operator!=(pixel_pos_view const & rhs) const {
		return !(*this == rhs);
	}

	pixel_pos_view begin() {return *this;}
	pixel_pos_view end() {return {};}

private:
	size_t _w, _h;
	pair<size_t, size_t> _pos;  //!< (column, row)
};  // pixel_pos_view

pixel_pos_view operator+(int n, pixel_pos_view const & pos) {
	return pos+n;
}

pixel_pos_view operator-(int n, pixel_pos_view const & pos) {
	return pos-n;
}

TEST_CASE("random access iterator should allow following expressions",
	"[random][iterator]") {

	pixel_pos_view pos1;  // default constructor
	*pos1;  // access position as (x,y) pair
	pos1->first;  // access x
	++pos1;  // pre increment
	pos1++;  // post increment
	pixel_pos_view pos2;
	pos1 == pos2;  // equal operator
	pos1 != pos2;  // not equal operator
	pixel_pos_view pos3{pos1};  // copy constructor
	pos3 = pos2;  // assign operator

	pixel_pos_view pos4{2,2};
	++pos4;
	++pos4;
	--pos4;  // pre decrement
	pos4--;  // post decrement

	pixel_pos_view pos5{5,5};
	pos5[3];  // n-th element access
	pos5 += 3;  // step n elements forward
	pos5 -= 3;  // step n elements backward
	pos5 + 3;  // nth next element
	3 + pos5;
	pos5 - 3;  // nth previous element
	pixel_pos_view pos6 = pos5+3;
	pos6 - pos5;  // distance
	pos5 < pos6;  // pos5 before pos6
	pos5 > pos6;  // pos5 after pos6
	pos5 <= pos6;  // pos5 not after pos6
	pos5 >= pos6;  // pos5 not before pos6
}

TEST_CASE("following should work for bidirectional iterator",
	"[random][iterator]") {

	pixel_pos_view pos1{2,3};

	SECTION("creation") {
		REQUIRE((*pos1 == pair<size_t, size_t>{0,0}));
		REQUIRE(pos1->first == 0);

		pixel_pos_view pos2;
		REQUIRE((*pos2 == pair<size_t, size_t>{0,0}));
	}

	SECTION("pre increment") {
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,0}));
		++pos1;
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
		REQUIRE((*(++pos1) == pair<size_t, size_t>{0,2}));
	}

	SECTION("post increment") {
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,0}));
		pixel_pos_view pos2 = pos1++;
		REQUIRE((*pos2 == pair<size_t, size_t>{1,0}));
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
	}

	SECTION("equal/not equall operators") {
		pixel_pos_view pos2{2, 3};
		REQUIRE(pos1 == pos2);
		++pos1;
		REQUIRE(pos1 != pos2);
		++pos2;
		REQUIRE(pos1 == pos2);

		pixel_pos_view pos3{1,1};
		++pos3;
		++pos3;
		++pos3;  // (1,1)
		++pos3;  // {}
		REQUIRE(pos3 == pixel_pos_view{});
	}

	SECTION("copy constructor") {
		++pos1; ++pos1; ++pos1;
		pixel_pos_view pos2{pos1};
		REQUIRE((*pos2 == pair<size_t, size_t>{1,1}));
	}

	SECTION("assign operator") {
		pixel_pos_view pos2;
		REQUIRE(pos1 != pos2);
		pos2 = pos1;
		REQUIRE(pos1 == pos2);
		++pos1;
		REQUIRE(pos1 != pos2);
		++pos2;
		REQUIRE(pos1 == pos2);
	}

	SECTION("pre decrement") {
		++pos1;
		++pos1;
		++pos1;
		++pos1;
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,2}));
		--pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{0,2}));
		--pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
		REQUIRE((*(--pos1) == pair<size_t, size_t>{0,1}));
	}

	SECTION("post decrement") {
		++pos1;
		++pos1;
		++pos1;
		++pos1;
		++pos1;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,2}));
		pos1--;
		REQUIRE((*pos1 == pair<size_t, size_t>{0,2}));
		pos1--;
		REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
		REQUIRE((*(pos1--) == pair<size_t, size_t>{1,1}));
	}

	SECTION("n-th element access") {}
	SECTION("step n elements forward") {}
	SECTION("step n elements backward") {}
	SECTION("n-th next element") {}
	SECTION("n-th previous element") {}
	SECTION("iterator distance") {}
	SECTION("less operator") {}
	SECTION("grater operator") {}
	SECTION("less-equal operator") {}
	SECTION("greater-equal operator") {}
}

TEST_CASE("we can convert view into iterator",
	"[iterator]") {
	pixel_pos_view pos;
	REQUIRE((*begin(pos) == pair<size_t, size_t>(0, 0)));
	REQUIRE(end(pos) == pixel_pos_view{});
}

uint8_t gradient_for(pair<size_t, size_t> pos, size_t w, size_t h) {
	double x = pos.first / double(w),
		y = pos.second / double(h),
		distance = sqrt(x*x + y*y);
	return static_cast<uint8_t>(ceil(255.0 * distance/sqrt(2.0)));
}

TEST_CASE("we can use transform with bidirectional iterator",
	"[bidirectional][iterator][transform]") {

	constexpr size_t w = 400,
		h = 300;

	uint8_t pixels[w*h] = {0};  // grayscale pixels
	pixel_pos_view pos{w, h};
	transform(begin(pos), end(pos), begin(pixels),
		[w, h](pair<size_t, size_t> const & pos){
			return gradient_for(pos, w, h);
		});

	REQUIRE(pixels[0] == 0x0);
	REQUIRE(pixels[w*h-1] == 0xff);
}

TEST_CASE("we can use parallel transform with bidirectional iterator",
	"[idirectional][iterator][parallel][transform]") {

	constexpr size_t w = 400,
		h = 300;

	uint8_t pixels[w*h] = {0};  // grayscale pixels
	pixel_pos_view pos{w, h};
	transform(std::execution::seq, begin(pos), end(pos), begin(pixels),
		[w, h](pair<size_t, size_t> const & pos){  // (column, row) position
			return gradient_for(pos, w, h);
		});

	REQUIRE(pixels[0] == 0x0);
	REQUIRE(pixels[w*h-1] == 0xff);
	save_grayscale(pixels, w, h, gradient_image);
}
