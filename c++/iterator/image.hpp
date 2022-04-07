/*! \file 
Image support. */
#pragma once
#include <filesystem>

void save_grayscale(uint8_t * pixels, size_t w, size_t h, std::filesystem::path const & as);
