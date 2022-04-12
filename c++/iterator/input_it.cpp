// Input iterator implementation sample.
#include <algorithm>
#include <utility>
#include <iterator>
#include <filesystem>
#include <cmath>
#include <cstdint>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "image.hpp"
#include "test_it.hpp"
using std::transform, std::pair, std::begin, std::end, std::filesystem::path;

path const gradient_image = "input_gradient.png";

/*! Input iterator view implemenation.
\note value_type is const which means reference is not mutable. */
struct pixel_pos_view 
	: public std::iterator<std::input_iterator_tag, pair<size_t, size_t> const> {
	
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
				r = _h;  // end of range (0, _h)
		}
		return *this;
	}

	void operator++(int) {
		++(*this);
	}

	//! comparison for for-each loop (works only for it == end(r) where r is pixel_pos_view instance)
	bool operator==(pixel_pos_view const & rhs) const {
		return _h == _pos.second && (rhs._w == 0 && rhs._h == 0);
	}

	bool operator!=(pixel_pos_view const & rhs) const {
		return !(*this == rhs);
	}

	pixel_pos_view begin() {return *this;}
	pixel_pos_view end() {return {};}

private:
	size_t _w, _h;
	pair<size_t, size_t> _pos;  //!< (column, row)
};



TEST_CASE("input iterator should allow following expressions", 
	"[input][iterator]") {
	REQUIRE(input_iterator_api_available<pixel_pos_view>());
}

TEST_CASE("following should work for input itetrator",
	"[input][iterator]") {
	input_iterator_api_implemented<pixel_pos_view> iter;

	SECTION("creation") {
		REQUIRE(iter.creation());
	}
	
	SECTION("pre increment") {
		REQUIRE(iter.pre_increment());
	}

	SECTION("post increment") {
		REQUIRE(iter.post_increment());
	}

	SECTION("equal/not equall operators") {
		REQUIRE(iter.equal());
	}

	SECTION("not equal operator") {
		REQUIRE(iter.not_equal());
	}

	SECTION("copy constructor") {
		REQUIRE(iter.copy_ctor());
	}
}

TEST_CASE("we can convert view into iterator", 
	"[input][iterator]") {
	pixel_pos_view pos;
	REQUIRE(begin(pos) == pos.begin());
	REQUIRE(end(pos) == pos.end());
}

TEST_CASE("we can use transform with input iterator", 
	"[input][iterator][transform]") {
	constexpr size_t w = 400,
		h = 300;

	uint8_t pixels[w*h] = {0};  // grayscale pixels 
	pixel_pos_view pos{w, h};
	transform(begin(pos), end(pos), begin(pixels),
		[w, h](pair<size_t, size_t> const & pos){  // (column, row) position
			double x = pos.first / double(w),
				y = pos.second / double(h),
				distance = sqrt(x*x + y*y);
			return static_cast<uint8_t>(ceil(255.0 * distance/sqrt(2.0)));
		});

	REQUIRE(pixels[0] == 0x0);
	REQUIRE(pixels[w*h-1] == 0xff);
	save_grayscale(pixels, w, h, gradient_image);
}
