#include <Magick++.h>
#include "png.hpp"
using std::filesystem::path;
using namespace Magick;


void save_grayscale_png(uint8_t * pixels, size_t w, size_t h, path const & as) {
	Image im;
	im.read(w, h, "GRAY", StorageType::CharPixel, pixels);
	im.write(as);
}
