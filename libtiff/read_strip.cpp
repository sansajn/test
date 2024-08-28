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
#include <string>
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

using std::span, std::string, std::vector, std::pair, std::unique_ptr, std::byte;
using std::filesystem::path, std::ifstream, std::cout;
using boost::format, boost::str;
using namespace Magick;

/*! Save to PNG image.
\param[in] fmt Use "I" for gray sacle and "RGB" for RGB images. */
template <typename PixelType>
void save_image(span<PixelType> pixels, size_t w, size_t h, string const & fmt, path const & fname) {
	Image im;

	static_assert(sizeof(PixelType) <= 2, "only 8 and 16bit images supported");
	auto storage_type = sizeof(PixelType) == 2 ? StorageType::ShortPixel : StorageType::CharPixel;
	im.read(w, h, fmt.c_str(), storage_type, pixels.data());
	
	im.write(fname);
}


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
	path const strip_path = "strip_image.png"; //str(format("strip_%1%.png") % strip_idx_arg);
	string const png_format = (samples_per_pixel == 3) ? "RGB" : "I";
	
	if (bits_per_sample == 16) {
		save_image(span<uint16_t>{
			reinterpret_cast<uint16_t *>(image_data.get()), image_w*image_h}, image_w, image_h, png_format, strip_path);
	}
	else if (bits_per_sample == 8) {
		save_image(span<uint8_t>{
			reinterpret_cast<uint8_t *>(image_data.get()), image_w*image_h}, image_w, image_h, png_format, strip_path);
	}

	// cout << "strip " << strip_idx_arg << " dumped as " << strip_path << "\n";

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
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
