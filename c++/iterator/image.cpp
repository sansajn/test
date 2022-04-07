#include <Magick++.h>
#include "image.hpp"
using std::filesystem::path;
using namespace Magick;


void save_grayscale(uint8_t * pixels, size_t w, size_t h, path const & as) {
	Image im;
	im.read(w, h, "I", StorageType::CharPixel, pixels);
	im.flip();
	im.write(as);
}
