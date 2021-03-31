// save pixel data as image sample
#include <vector>
#include <string>
#include <iostream>
#include <Magick++.h>
using std::vector, std::string, std::cout;
using namespace Magick;

void save_image(vector<uint8_t> const & pixels_rgb, size_t w, size_t h, string const & fname) {
	Image im;
	im.read(w, h, "RGB", StorageType::CharPixel, pixels_rgb.data());
	im.write(fname);
}

int main(int argc, char * argv[]) {
	// create black/white checker
	constexpr size_t square_count = 9;
	constexpr size_t w = 5 * square_count;
	vector<uint8_t> pixels(w * w * 3, 0x0);
	for (size_t r = 0; r < w; ++r)
		for (size_t c = 0; c < w; ++c) {
			size_t idx = (r*w + c)*3;
			uint8_t * p = &pixels[idx];
			if (idx % 2) {
				*p++ = 0xff;
				*p++ = 0xff;
				*p = 0xff;
			}
		}

	save_image(pixels, w, w, "out.png");
	
	cout << "checker saved as 'out.png'\n";

	cout << "done!\n";
	return 0;
}
