// Input iterator implementation sample.
#include <algorithm>
#include <utility>
#include <iterator>
#include <cmath>
#include <cstdint>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "png.hpp"
using std::transform, std::pair, std::begin, std::end;

//! Input iterator view implemenation.
struct pixel_pos_view 
	: public std::iterator<std::input_iterator_tag, pair<size_t, size_t>> {
	
	pixel_pos_view() : pixel_pos_view{0, 0} {}
	
	pixel_pos_view(size_t w, size_t h) 
		: _w{w}, _h{h}, _pos{0, 0} 
	{}

	pair<size_t, size_t> const & operator*() {  // we do not want to allow modification there
		return _pos;
	}

	pair<size_t, size_t> const * operator->() {
		return &_pos;
	}

	void operator++() {
		auto & [c, r] = _pos;
		c += 1;
		if (c >= _w) {
			c = 0;

			r += 1;
			if (r >= _h)
				r = _h;  // end of range
		}
	}

	void operator++(int) {
		++(*this);
	}

	//! comparison for for-each loop (works only for it == end(r) where r is pixel_pos_view instance)
	bool operator==(pixel_pos_view const & rhs) {
		return _h == _pos.second && (rhs._w == 0 && rhs._h == 0);
	}

	bool operator!=(pixel_pos_view const & rhs) {
		return !(*this == rhs);
	}

private:
	size_t _w, _h;
	pair<size_t, size_t> _pos;  //!< (column, row)
};


TEST_CASE("input iterator should allow following expressions", 
	"[input-iterator]") {
	
	pixel_pos_view pos1, pos2;
	*pos1;  // access position as (x,y) pair
	pos1->first;  // access x
	++pos1;  // pre increment
	pos1++;  // post increment
	pos1 == pos2;  // equal operator
	pos1 != pos2;  // not equal operator
	pixel_pos_view pos3{pos1};  // copy constructor
}

TEST_CASE("following should be true for input itetrator", 
	"[input-iterator]") {

		pixel_pos_view pos1{2, 3};

		SECTION("creation") {
			REQUIRE((*pos1 == pair<size_t, size_t>{0,0}));
			REQUIRE(pos1->first == 0);
		}
		
		SECTION("pre increment") {
			++pos1;
			REQUIRE((*pos1 == pair<size_t, size_t>{1,0}));
			++pos1;
			++pos1;
			REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
		}

		SECTION("post increment") {
			++pos1;
			REQUIRE((*pos1 == pair<size_t, size_t>{1,0}));
			++pos1;
			++pos1;
			REQUIRE((*pos1 == pair<size_t, size_t>{1,1}));
		}

		SECTION("equal/not equall operators") {
			pixel_pos_view pos2;
			REQUIRE(pos1 != pos2);

			pixel_pos_view pos3;
			REQUIRE(pos2 == pos3);
		}

		SECTION("copy constructor") {
			++pos1; ++pos1; ++pos1;
			pixel_pos_view pos2{pos1};
			REQUIRE((*pos2 == pair<size_t, size_t>{1,1}));
		}
}



/*
int main(int argc, char * argv[]) {
	constexpr size_t w = 400,
		h = 300;

	// input iterator requirements
	pixel_pos_view pos1, pos2;
	*pos1;  // access position as (x,y) pair
	pos1->first;  // access x
	++pos1;  // pre increment
	pos1++;  // post increment
	pos1 == pos2;  // equal operator
	pos1 != pos2;  // not equal operator
	pixel_pos_view pos3{pos1};  // copy constructor

	// input iterator final test
	uint8_t pixels[w*h];  // grayscale pixels 
	pixel_pos_view pos{w, h};
	transform(begin(pos), end(pos), begin(pixels),
		[w, h](pair<size_t, size_t> const & pos){  // (column, row) position
			double x = pos.first / double(w),
				y = pos.second / double(h),
				distance = sqrt(x*x + y*y);
			return static_cast<uint8_t>(255.0 * distance/sqrt(2.0));
		});

	save_grayscale_png("input.png");

	return 0;
}
*/
