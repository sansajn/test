/*! Create gray 8bit view from array of pixels and save it to a png file.
Tested with Boost 1.74 */
#include <vector>
#include <iostream>
#include <iomanip>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>

using std::vector;
using std::cout;
using boost::gil::gray8_view_t, boost::gil::gray8_pixel_t, boost::gil::interleaved_view,
	boost::gil::write_view, boost::gil::png_tag;

using boost::gil::gray8_pixel_t, boost::gil::at_c;

void print_pixels(gray8_view_t & pixels, size_t w, size_t h) {
	uint8_t const * pixel_data = (uint8_t *)&(*pixels.begin());
	for (size_t r = 0; r < h; ++r) {
		for (size_t c = 0; c < w; ++c)
			cout << std::hex << size_t(pixel_data[r*w + c]) << ' ';
		cout << '\n';
	}
}

void print_pixels(uint8_t const * pixels, size_t w, size_t h) {
	for (size_t r = 0; r < h; ++r) {
		for (size_t c = 0; c < w; ++c)
			cout << std::hex << size_t(pixels[r*w + c]) << ' ';
		cout << '\n';
	}
}

int main(int argc, char * argv[]) {
	vector<uint8_t> const pixels{
		0x11, 0x22, 
		0x33, 0x44};
	size_t const w = 2,
		h = 2;

	gray8_view_t pixel_view = interleaved_view(w, h, (gray8_pixel_t *)pixels.data(), w*sizeof(gray8_pixel_t));
	print_pixels(pixel_view, w, h);

	// copy flipped_view into a pixel array
	vector<uint8_t> flipped_pixels(w*h);
	gray8_view_t flipped_view = flipped_up_down_view(pixel_view);
	copy_pixels(flipped_view, interleaved_view(w, h, (gray8_pixel_t *)flipped_pixels.data(), w*sizeof(gray8_pixel_t)));
	print_pixels(flipped_pixels.data(), w, h);

	return 0;
}
