// figure out strip count in a TIFF file sample
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

	unsigned const strip_count = TIFFNumberOfStrips(tiff);
	cout << "strips=" << strip_count << "\n";

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}
