#pragma once
#include <vector>
#include <cstddef>

//! Create grayscale checker (square) texture and returns it's size in pixels.
size_t create_checker_texture(std::vector<std::byte> & pixels, size_t square_count = 9,
	size_t square_width = 5);
