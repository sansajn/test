#include "checker.hpp"
using std::vector, std::byte;

namespace {

template <typename T>
size_t create_checker(T color1, T color2, size_t square_count, size_t square_width,
	vector<T> & pixels) {

	size_t const w = square_count * square_width;

	pixels.resize(w*w);

	for (size_t r = 0; r < square_count; ++r) {
		for (size_t c = 0; c < square_count; ++c) {
			// choose color
			T color;
			if (r % 2)
				color = c % 2 ? color1 : color2;
			else
				color = c % 2 ? color2 : color1;

			// write
			for (size_t i = 0; i < square_width; ++i) {
				size_t const idx = (r*square_count)*square_width*square_width + c*square_width + i*w;
				for (size_t j = 0; j < square_width; ++j)
					pixels[idx + j] = color;
			}
		}
	}

	return w;
}

}  // namespace

size_t create_checker_texture(vector<byte> & pixels, size_t square_count, size_t square_width) {
	constexpr byte black{0x0},
		white{0xff};

	return create_checker(black, white, square_count, square_width, pixels);
}

size_t create_checker16_texture(std::vector<uint16_t> & pixels, size_t square_count,
	size_t square_width) {

	constexpr uint16_t black{0x0},
		white{0xffff};

	return create_checker(black, white, square_count, square_width, pixels);
}
