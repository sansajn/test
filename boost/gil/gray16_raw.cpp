//! Create gray 16bit view from array of pixels and save it to a png file (working with boost 1.74).
#include <vector>
#include <cstdint>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>

using std::vector;
using boost::gil::gray16_view_t, boost::gil::gray16_pixel_t, boost::gil::interleaved_view,
	boost::gil::write_view, boost::gil::png_tag;

int main(int argc, char * argv[]) {
	vector<uint16_t> const pixels{0, 0xffff, 0xffff, 0};
	size_t const w = 2,
		h = 2;

	gray16_view_t pixel_view = interleaved_view(w, h, (gray16_pixel_t *)pixels.data(), w*sizeof(uint16_t));
	write_view("test.png", pixel_view, png_tag());

	return 0;
}
