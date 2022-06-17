//! Create gray 8bit view from array of pixels and save it to a png file (working with boost 1.74).
#include <vector>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>

using std::vector;
using boost::gil::gray8_view_t, boost::gil::gray8_pixel_t, boost::gil::interleaved_view,
	boost::gil::write_view, boost::gil::png_tag;

int main(int argc, char * argv[]) {
	vector<uint8_t> const pixels{0, 0xff, 0xff, 0};
	size_t const w = 2,
		h = 2;

	gray8_view_t pixel_view = interleaved_view(w, h, (gray8_pixel_t *)pixels.data(), w);
	write_view("test.png", pixel_view, png_tag());

	return 0;
}
