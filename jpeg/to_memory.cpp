// citanie a zapis do pamete
#include <string>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <jpeglib.h>

using std::string;
using std::cout;

uint8_t * read_image(uint8_t * data, size_t size, size_t & w, size_t & h);  //!< read from memory


int main(int argc, char * argv[])
{
	FILE * fimg = fopen("lena.jpg", "rb");
	assert(fimg);

	fseek(fimg, 0, SEEK_END);
	size_t size = ftell(fimg);

	fseek(fimg, 0, SEEK_SET);
	uint8_t * data = new uint8_t[size];
	fread(data, 1, size, fimg);

	fclose(fimg);
	fimg = nullptr;

	size_t w, h;
	uint8_t * pixels = read_image(data, size, w, h);

	cout << "w:" << w << ", h:" << h << "\n";

	delete [] data;

	return 0;
}



uint8_t * read_image(uint8_t * data, size_t size, size_t & w, size_t & h)
{
	assert(data);

	jpeg_decompress_struct cinfo;

	// error handling
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);

	// read jpeg
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, data, size);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	static_assert(BITS_IN_JSAMPLE == 8, "supports only 8-bit jpeg");
	static_assert(std::is_same<JSAMPLE, uint8_t>::value, "JSAMPLE is not uint8_t, pixels buffer type not match");

	unsigned rowbytes = cinfo.output_width * cinfo.output_components;
	uint8_t * pixels = new uint8_t[rowbytes * cinfo.output_height];

	uint8_t ** rows = new uint8_t * [cinfo.output_height];
	for (unsigned r = 0; r < cinfo.output_height; ++r)
		rows[r] = pixels + r*rowbytes;

	while (cinfo.output_scanline < cinfo.output_height)
	{
		JSAMPARRAY dst = rows + cinfo.output_scanline;  // byte **
		jpeg_read_scanlines(&cinfo, dst, cinfo.output_height);  // read whole picture
	}

	w = cinfo.output_width;
	h = cinfo.output_height;

	// clenup
	delete [] rows;
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return pixels;
}
