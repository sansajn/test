/*! \file 
Image support. */
#pragma once
#include <filesystem>
#include <utility>

void save_grayscale(uint8_t * pixels, size_t w, size_t h, std::filesystem::path const & as);

uint8_t gradient_for(std::pair<size_t, size_t> pos, size_t w, size_t h);  // TODO: move it into gradient module
