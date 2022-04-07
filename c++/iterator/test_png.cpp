#include <filesystem>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "png.hpp"

namespace fs = std::filesystem;

TEST_CASE("we can save grayscale pixel array as png file", 
	"[png][save_grayscale_png]") {

	constexpr size_t w = 300, 
		h = 300;
	uint8_t pixels[w*h] = {0};

	// create grayscale gradient
	for (size_t c = 0; c < w; ++c) {
		for (size_t r = 0; r < h; ++r) {
			double x = c / double(w),
			y = r / double(h),
			distance = sqrt(x*x + y*y);
			pixels[c + r*w] = 255;  //static_cast<uint8_t>(255.0 * distance/sqrt(2.0));
		}
	}

	save_grayscale_png(pixels, w, h, "pixels.bmp");

	REQUIRE(fs::exists("pixels.png"));
}
