/* Strip-oriented Image reading sample see [Strip-oriented Image I/O](http://www.libtiff.org/libtiff.html#strip-oriented-image-i-o).
The program reads tiff 16b file strip afer the strip and save it into 
one png file (`strip_image.png`). To verify image you need to stretch 
color levels e.g. in Gimp.
usage: read_tile TIFF_FILE=test.tiff */
#include <span>
#include <filesystem>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <boost/format.hpp>  // note STL <format> not yet available
#include <tiffio.hxx>
#include <Magick++.h>

using std::span, std::vector, std::pair, std::unique_ptr, std::byte;
using std::filesystem::path, std::ifstream, std::cout;
using boost::format, boost::str;
using namespace Magick;

void save_image_gray(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname);
void save_image_rgb(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname);

//! Create grayscale test image and returns dimension as (width, height) pair.
pair<size_t, size_t> create_test_image(vector<byte> & pixels);
pair<size_t, size_t> create_test_image_16(vector<uint16_t> & pixels);


int main(int argc, char * argv[]) {
	path tiff_path_arg = "test.tiff";
	if (argc > 1)
		tiff_path_arg = argv[1];

	ifstream fin{tiff_path_arg};
	assert(fin.is_open());

	TIFF * tiff = TIFFStreamOpen("memory", &fin);
	assert(tiff);

	// image w, h
	uint32_t image_w = 0, 
		image_h = 0;
	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &image_w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &image_h);
	cout << "image-w=" << image_w << '\n'
		<< "image-h=" << image_w << '\n';

	// tiles
	uint32_t const tile_count = TIFFNumberOfTiles(tiff);
	cout << "tiles=" << tile_count << "\n";

	// tile-width/height?
	uint32_t tile_w = 0,
		tile_h = 0;
	TIFFGetField(tiff, TIFFTAG_TILEWIDTH, &tile_w);
	TIFFGetField(tiff, TIFFTAG_TILELENGTH, &tile_h);

	cout << "tile-w=" << tile_w << "\n"
		<< "tile-h=" << tile_h << "\n";

	// strips
	tstrip_t const strip_count = TIFFNumberOfStrips(tiff);
	cout << "strips=" << strip_count << '\n';

	//assert(strip_idx_arg < strip_count && "out of strip index");

	tmsize_t const strip_size = TIFFStripSize(tiff);  // in bytes
	cout << "strip-size=" << strip_size << '\n';

	uint32_t rows_per_strip = 0;
	TIFFGetField(tiff, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
	cout << "rows-per-strip=" << rows_per_strip << '\n';

	// pixel format?
	uint16_t bits_per_sample = 0;
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	cout << "bits-per-sample=" << bits_per_sample << "\n";

	uint16_t sample_format = 0;
	TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sample_format);

	cout << "sample-format=";
	switch (sample_format) {
		case SAMPLEFORMAT_UINT: cout << "UINT"; break;
		case SAMPLEFORMAT_INT: cout << "INT"; break;
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

	assert(tile_h == 0 && tile_w == 0 && "we expect strips not tiles");

	// read strip by strip and store it into whole image
	unsigned const image_size = image_w*image_h*(bits_per_sample/8)*samples_per_pixel;
	unique_ptr<byte> image_data{new byte[image_size]};
	memset(image_data.get(), 0xff, image_size);   // clear buffer
	for (tstrip_t strip = 0; strip < strip_count; ++strip) {
		unsigned const offset = strip*strip_size;
		uint8_t * buf = reinterpret_cast<uint8_t *>(image_data.get()) + offset;
		cout << "#" << std::dec << strip << ". buf=" << std::hex << uint64_t(buf) << ", offset=" << offset << '\n';
		tmsize_t ret = TIFFReadEncodedStrip(tiff, strip, buf, (tsize_t)-1);
		assert(ret != -1 && ret > 0);
	}

	// now save to png in case of 16bit tif images
	// TODO: implement saving also 8bit images and RGB files
	if (bits_per_sample == 16) {
		path const strip_path = "strip_image.png"; //str(format("strip_%1%.png") % strip_idx_arg);
		if (samples_per_pixel == 1)
			save_image_gray(span<uint16_t>{
				reinterpret_cast<uint16_t *>(image_data.get()), image_w*image_h}, image_w, image_h, strip_path);
		else
			save_image_rgb(span<uint16_t>{
				reinterpret_cast<uint16_t *>(image_data.get()), image_w*image_h}, image_w, image_h, strip_path);

		// cout << "strip " << strip_idx_arg << " dumped as " << strip_path << "\n";
	}

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}

void save_image_rgb(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname) {
	Image im;
	im.read(w, h, "RGB", StorageType::ShortPixel, pixels_gray.data());
	im.write(fname);
}

void save_image_gray(span<uint16_t> pixels_gray, size_t w, size_t h, path const & fname) {
	Image im;
	im.read(w, h, "I", StorageType::ShortPixel, pixels_gray.data());
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

pair<size_t, size_t> create_test_image_16(vector<uint16_t> & pixels) {
	constexpr size_t square_count = 9,
		w = 5 * square_count;
	
	pixels.resize(w*w);
	
	// create black/white checker
	for (size_t r = 0; r < w; ++r) {
		for (size_t c = 0; c < w; ++c) {
			size_t idx = r*w + c;
			if (idx % 2)
				pixels[idx] = uint16_t{0xffff};
			else
				pixels[idx] = uint16_t{0x0};
		}
	}

	return {w, w};
}
