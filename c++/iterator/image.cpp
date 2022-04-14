#include <cmath>
#include <Magick++.h>
#include "image.hpp"
using std::pair, std::filesystem::path;
using namespace Magick;


void save_grayscale(uint8_t * pixels, size_t w, size_t h, path const & as) {
	Image im;
	im.read(w, h, "I", StorageType::CharPixel, pixels);
	im.flip();
	im.write(as);
}

uint8_t gradient_for(pair<size_t, size_t> pos, size_t w, size_t h) {
	double x = pos.first / double(w),
		y = pos.second / double(h),
		distance = sqrt(x*x + y*y);
	return static_cast<uint8_t>(ceil(255.0 * distance/sqrt(2.0)));
}
