#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>

//! Create 8bit grayscale checker (square) texture and returns it's size (width is the same as height) in pixels.
size_t create_checker_texture(std::vector<std::byte> & pixels, size_t square_count = 9,
	size_t square_width = 5);

//! 16bit create_checker_texture() version.
size_t create_checker16_texture(std::vector<uint16_t> & pixels, size_t square_count = 9,
	size_t square_width = 5);
