// figure out number of directories in a TIFF file
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

	int dir_count = 0;
	do {
		++dir_count;
	} while (TIFFReadDirectory(tiff));
	
	cout << "directories=" << dir_count << "\n";

	TIFFClose(tiff);

	cout <<"done!\n";

	return 0;
}
