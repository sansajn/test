// PPM (RGB) image writer sample.
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
using std::filesystem::path, std::vector, std::ofstream, std::cout;

/*! Write RGB PPM image to file. 
\note Origin for the image is left top corner (so the data is written from the top to bottom). */
bool write_ppm(int w, int h, vector<uint8_t> const & pixels, path const & fn);

int main(int argc, char * argv[]) {
	constexpr int w = 300, 
		h = 300;

	// generate RGB image data
	vector<uint8_t> pixels(w*h*3);
	
	uint8_t * p = pixels.data();
	for (int r = 0; r < h; ++r) {
		uint8_t const g = uint8_t(r/float(h) * 255.0f);
		
		for (int c = 0; c < w; ++c) {
			*(p++) = uint8_t(c/float(w) * 255.0f);  // R
			*(p++) = g;  // G
			*(p++) = 0; // B
		}
	}
	
	write_ppm(w, h, pixels, "out.ppm");
	
	cout << "'out.ppm' saved, done!\n";
	return 0;
}

bool write_ppm(int w, int h, vector<uint8_t> const & pixels, path const & fn) {
	ofstream of;
	of.open(fn, std::ios::out|std::ios::binary);
	if (!of.is_open())
		return false;
	
	of << "P6\n" << w << " " << h << "\n255\n";
	for (uint8_t p : pixels)
		of << p;
	
	of.close();
	return true;
}
