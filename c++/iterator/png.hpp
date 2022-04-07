#pragma once
#include <filesystem>

void save_grayscale_png(uint8_t * pixels, size_t w, size_t h, std::filesystem::path const & as);
