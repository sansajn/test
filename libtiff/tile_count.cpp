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

	uint32 image_w, 
		image_h, 
		tile_w, 
		tile_h;

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

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}