/* Tile-oriented Image reading sample see [Tile-oriented Image I/O](http://www.libtiff.org/libtiff.html#Tiles).
The program reads 300th tile (from Landsat 8 tile).
usage: read_tile TIFF_FILE [TILE_IDX] */
#include <span>
#include <filesystem>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <tiffio.hxx>
#include <Magick++.h>

using std::span, std::vector, std::pair, std::byte;
using std::filesystem::path, std::ifstream, std::cout;
using namespace Magick;

void save_image(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname);

//! Create grayscale test image and returns dimension as (width, height) pair.
pair<size_t, size_t> create_test_image(vector<byte> & pixels);

int main(int argc, char * argv[]) {
	path tiff_path_arg = "test.tiff";
	if (argc > 1)
		tiff_path_arg = argv[1];

	int tile_idx_arg = 300;
	if (argc > 2)
		tile_idx_arg = atoi(argv[2]);

	ifstream fin{tiff_path_arg};
	assert(fin.is_open());

	TIFF * tiff = TIFFStreamOpen("memory", &fin);
	assert(tiff);

	// tiles
	uint32_t const tile_count = TIFFNumberOfTiles(tiff);
	cout << "tiles=" << tile_count << "\n";
	assert(tile_idx_arg < tile_count);

	// tile-width/height?
	uint32_t tile_w = 0,
		tile_h = 0;
	TIFFGetField(tiff, TIFFTAG_TILEWIDTH, &tile_w);
	TIFFGetField(tiff, TIFFTAG_TILELENGTH, &tile_h);

	cout << "tile-w=" << tile_w << "\n"
		<< "tile-h=" << tile_h << "\n";

	// pixel format?
	uint16_t bits_per_sample = 0;
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	cout << "bits-per-sample=" << bits_per_sample << "\n";

	uint16_t sample_format = 0;
	TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sample_format);

	cout << "sample-format=";
	switch (sample_format) {
		case SAMPLEFORMAT_UINT: cout << "UINT"; break;  // TODO: how many bits?
		default: cout << sample_format;
	}
	cout << "\n";

	uint16_t photometric = 0;
	TIFFGetField(tiff, TIFFTAG_PHOTOMETRIC, &photometric);
	cout << "photometric=";
	switch (photometric) {
		case PHOTOMETRIC_MINISBLACK: cout << "grayscale with min-is-black"; break;
		default: cout << photometric;
	}
	cout << "\n";

	uint16_t samples_per_pixel = 0;
	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
	cout << "samples-per-pixel=" << samples_per_pixel << "\n";

	uint16_t planar_config = 0;
	TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &planar_config);
	cout << "planar-configuration=";
	switch (planar_config) {
		case PLANARCONFIG_CONTIG: cout << "single image plane (" << planar_config << ")"; break;
		case PLANARCONFIG_SEPARATE: cout << "separate planes of data (" << planar_config << ")"; break;
	}
	cout << "\n";

	size_t const tile_size = TIFFTileSize(tiff);
	byte * tile_data = new byte[tile_size];  // TODO: use unique_ptr there FIXME: we use 16biut UNIT format
	
	int ret = TIFFReadEncodedTile(tiff, tile_idx_arg, tile_data, size_t(-1));
	assert(ret != -1);

	assert(tile_size % 2 == 0);
	save_image(span<uint16_t>{reinterpret_cast<uint16_t *>(tile_data), tile_size/2}, tile_w, tile_h, "tile.png");
	cout << "tile " << tile_idx_arg << " dumped as \"tile.png\"\n";

	delete [] tile_data;

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}

void save_image(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname) {
	Image im;
	im.read(w, h, "I", StorageType::/*CharPixel*/ShortPixel, pixels_gray.data());
	im.write(fname);
}

pair<size_t, size_t> create_test_image(vector<byte> & pixels) {
	constexpr size_t square_count = 9,
		w = 5 * square_count;
	
	pixels.resize(w*w);
	
	// create black/white checker
	for (size_t r = 0; r < w; ++r) {
		for (size_t c = 0; c < w; ++c) {
			size_t idx = r*w + c;
			if (idx % 2)
				pixels[idx] = byte{0xff};
		}
	}

	return {w, w};
}
