// Forward iterator implementation sample.
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
#include "test_it.hpp"
using std::transform, std::pair, std::begin, std::end, std::filesystem::path;

path const gradient_image = "forward_gradient.png";

//! Forward iterator view implemenation.
struct pixel_pos_view 
	: public std::iterator<std::forward_iterator_tag, pair<size_t, size_t> const> {
	
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

	bool operator==(pixel_pos_view const & rhs) const {
		return (_w == rhs._w && _h == rhs._h && _pos == rhs._pos)
			|| (end_of_range() && rhs.end_of_range());
	}

	bool operator!=(pixel_pos_view const & rhs) const {
		return !(*this == rhs);
	}

	pixel_pos_view begin() {return *this;}
	pixel_pos_view end() {return {};}

private:
	bool end_of_range() const {return _pos.second == _h;}

	size_t _w, _h;
	pair<size_t, size_t> _pos;  //!< (column, row)
};

TEST_CASE("forward iterator should allow following expressions",
	"[forward][iterator]") {
	REQUIRE(forward_iterator_api_available<pixel_pos_view>());
}

TEST_CASE("following should be true for forward itetrator",
	"[forward][iterator]") {
	forward_iterator_api_implemented<pixel_pos_view> iter;

	SECTION("creation") {
		REQUIRE(iter.creation());
	}
	
	SECTION("pre increment") {
		REQUIRE(iter.pre_increment());
	}

	SECTION("post increment") {
		REQUIRE(iter.post_increment());
	}

	SECTION("equal operator") {
		REQUIRE(iter.equal());
	}

	SECTION("not equal operators") {
		REQUIRE(iter.not_equal());
	}

	SECTION("copy constructor") {
		REQUIRE(iter.copy_ctor());
	}

	SECTION("assign operator") {
		REQUIRE(iter.assign());
	}
}

TEST_CASE("we can convert view into iterator", 
	"[forward][iterator]") {
	pixel_pos_view pos;
	REQUIRE((*begin(pos) == pair<size_t, size_t>(0, 0)));
	REQUIRE(end(pos) == pixel_pos_view{});
}

TEST_CASE("we can use transform with forward iterator",
	"[forward][iterator][transform]") {
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

TEST_CASE("we can use parallel transform with forward iterator",
	"[forward][iterator][parallel][transform]") {
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
