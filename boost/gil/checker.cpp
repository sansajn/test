#include "checker.hpp"
using std::vector, std::byte;

size_t create_checker_texture(vector<byte> & pixels, size_t square_count,
	size_t square_width) {

	size_t const w = square_count * square_width;

	pixels.resize(w*w);

	constexpr byte black{0x0},
		white{0xff};

	for (size_t r = 0; r < square_count; ++r) {
		for (size_t c = 0; c < square_count; ++c) {
			// choose color
			byte color;
			if (r % 2)
				color = c % 2 ? black : white;
			else
				color = c % 2 ? white : black;

			// write
			size_t row_bytes = w;
			for (size_t i = 0; i < square_width; ++i) {
				size_t const idx = (r*square_count)*square_width*square_width + c*square_width + i*row_bytes;
				for (int j = 0; j < square_width; ++j)
					pixels[idx + j] = color;
			}
		}
	}

	return w;
}
