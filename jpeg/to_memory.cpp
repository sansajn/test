// citanie a zapis do pamete
#include <string>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <jpeglib.h>

using std::string;
using std::cout;

uint8_t * read_image(uint8_t * data, size_t size, size_t & w, size_t & h);  //!< read from memory

/*! write jpeg image to memory
\param[in] pixels RGB pixel data
\param[in] w image width
\param[in] h image height
\param[in] q compression quality 0 (worst) to 100 (best)
\param[out] size size of the image in bytes
\return returns pointer to a data, use free() to release the data */
uint8_t * write_image(uint8_t * pixels, size_t w, size_t h, int q, size_t & size);

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

	cout << "read from memory\n";
	size_t w, h;
	uint8_t * pixels = read_image(data, size, w, h);

	cout << "w:" << w << ", h:" << h << "\n";

	{
		cout << "write to memory\n";
		size_t size;
		uint8_t * data = write_image(pixels, w, h, 80, size);

		cout << "read from memory\n";
		size_t w, h;
		uint8_t * pixels = read_image(data, size, w, h);
		cout << "w:" << w << ", h:" << h << "\n";
		delete [] pixels;

		free(data);
	}


	delete [] pixels;

	free(data);

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

uint8_t * write_image(uint8_t * pixels, size_t w, size_t h, int q, size_t & size)
{
	jpeg_compress_struct cinfo;

	jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];      // pointer to JSAMPLE row[s]
	int row_stride;               // physical row width in image buffer

	// Step 1: allocate and initialize JPEG compression object
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	// Step 2: specify data destination
	uint8_t * data = nullptr;
	jpeg_mem_dest(&cinfo, &data, &size);

	// Step 3: set parameters for compression

	cinfo.image_width = w;                // image width and height, in pixels
	cinfo.image_height = h;
	cinfo.input_components = 3;           // # of color components per pixel
	cinfo.in_color_space = JCS_RGB;       // colorspace of input image

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, q, TRUE /* limit to baseline-JPEG values */);

	// Step 4: Start compressor
	jpeg_start_compress(&cinfo, TRUE);

	// Step 5: while (scan lines remain to be written)
	//           jpeg_write_scanlines(...); */
	row_stride = w * 3; // JSAMPLEs per row in image_buffer

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &pixels[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// Step 6: Finish compression

	jpeg_finish_compress(&cinfo);

	// Step 7: release JPEG compression object

	jpeg_destroy_compress(&cinfo);

	return data;
}
