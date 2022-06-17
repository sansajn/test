/*! 2D checker texture generator sample.
Usage: checker [SQUARE_COUNT=9 [SQUARE_WIDTH=5]] */
#include <vector>
#include <span>
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include "checker.hpp"

using std::vector, std::span, std::byte;
using std::cout;
using boost::gil::gray8_view_t, boost::gil::gray8_pixel_t, boost::gil::interleaved_view,
	boost::gil::write_view, boost::gil::png_tag;

int main(int argc, char * argv[]) {
	int square_count = 9;
	if (argc > 1)
		square_count = atoi(argv[1]);
	assert(square_count > 0);

	int square_width = 5;
	if (argc > 2)
		square_width = atoi(argv[2]);
	assert(square_width > 0);

	cout << "square_count=" << square_count << "\n"
		<< "square_width=" << square_width << "\n";

	vector<byte> pixels;
	size_t const w = create_checker_texture(pixels, static_cast<size_t>(square_count),
		static_cast<size_t>(square_width));

	gray8_view_t pixel_view = interleaved_view(w, w, (gray8_pixel_t *)pixels.data(), w);
	write_view("checker.png", pixel_view, png_tag());

	cout << "checker.png saved\n"
		<<"done!\n";

	return 0;
}
