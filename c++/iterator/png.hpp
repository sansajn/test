#pragma once
#include <filesystem>

void save_grayscale_png(uint8_t * pixels, size_t size, std::filesystem::path const & as);
