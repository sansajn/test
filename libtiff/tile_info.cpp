// figure out tile count in a TIFF file sample, see [Tile-oriented Image I/O](http://www.libtiff.org/libtiff.html#Tiles)
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <tiffio.hxx>

using std::filesystem::path, std::ifstream, std::cout;

int main(int argc, char * argv[]) {
	path tiff_path = "test.tiff";
	if (argc > 1)
		tiff_path = argv[1];

	ifstream fin{tiff_path};
	assert(fin.is_open());

	TIFF * tiff = TIFFStreamOpen("memory", &fin);
	assert(tiff);

	uint32_t image_w = 0, 
		image_h = 0, 
		tile_w = 0, 
		tile_h = 0;

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &image_w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &image_h);
	TIFFGetField(tiff, TIFFTAG_TILEWIDTH, &tile_w);
	TIFFGetField(tiff, TIFFTAG_TILELENGTH, &tile_h);

	size_t tile_size = TIFFTileSize(tiff);
	size_t tile_count = TIFFNumberOfTiles(tiff);
		
	cout << "image-width=" << image_w << "\n"
		<< "image-length=" << image_h << "\n"
		<< "tile-width=" << tile_w << "\n"
		<< "tile-length=" << tile_h << "\n"
		<< "tile-size=" << tile_size << "\n"
		<< "tile-count=" << tile_count << "\n";

	// TODO: how to recognize data are saved as strips or tiles?

	// number of strips
	unsigned const strip_count = TIFFNumberOfStrips(tiff);
	cout << "strips=" << strip_count << "\n";

	// pixel format?
	uint16_t bits_per_sample = 0;
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
	cout << "bits-per-sample=" << bits_per_sample << "\n";

	uint16_t sample_format = 0;
	TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &sample_format);

	cout << "sample-format=";
	switch (sample_format) {
		case SAMPLEFORMAT_UINT: cout << "UINT"; break;
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

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}