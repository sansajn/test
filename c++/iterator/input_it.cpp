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

	void operator++() {}

	void operator++(int) {}

	bool operator==(pixel_pos_view const & rhs) {
		return false;
	}

	bool operator!=(pixel_pos_view const & rhs) {
		return !(*this == rhs);
	}

private:
	size_t _w, _h;
	pair<size_t, size_t> _pos;
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
