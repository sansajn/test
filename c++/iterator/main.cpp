// Input iterator implementation sample.
#include <algorithm>
#include <filesystem>
#include <utility>
#include <cmath>
#include <cstdint>
using std::transform, std::pair, std::begin, std::end;
using std::filesystem::path;

//! Input iterator view implemenation.
struct pixel_pos_view {
	pixel_pos_view(size_t w, size_t h) : _pos{w, h} {}

private:
	pair<size_t, size_t> _pos;
};

void save_grayscale_png(uint8_t * pixels, size_t size, path const & as);

int main(int argc, char * argv[]) {
	constexpr size_t w = 400,
		h = 300;
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

void save_grayscale_png(uint8_t * pixels, size_t size, path const & as) {

}
