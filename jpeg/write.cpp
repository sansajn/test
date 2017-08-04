#include <string>
#include <cstdio>
#include <cstdlib>
#include <jpeglib.h>
#include <Magick++.h>

using std::string;
using namespace Magick;


static void im_read(string const & fname, Blob & data);


int main(int argc, char * argv[])
{
	int const quality = 80;
	char const * filename = "out.jpg";
	int const image_width = 512;
	int const image_height = 512;

	// prepare input data
	Blob jpeg_data;
	im_read("lena.jpg", jpeg_data);
	uint8_t * image_buffer = (uint8_t *)jpeg_data.data();

	jpeg_compress_struct cinfo;

	jpeg_error_mgr jerr;
	FILE * outfile;               // target file
	JSAMPROW row_pointer[1];      // pointer to JSAMPLE row[s]
	int row_stride;               // physical row width in image buffer

	// Step 1: allocate and initialize JPEG compression object
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	// Step 2: specify data destination (eg, a file)
	if ((outfile = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}

	jpeg_stdio_dest(&cinfo, outfile);

	// Step 3: set parameters for compression

	cinfo.image_width = image_width;      // image width and height, in pixels
	cinfo.image_height = image_height;
	cinfo.input_components = 3;           // # of color components per pixel
	cinfo.in_color_space = JCS_RGB;       // colorspace of input image

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	// Step 4: Start compressor
	jpeg_start_compress(&cinfo, TRUE);

	// Step 5: while (scan lines remain to be written)
	//           jpeg_write_scanlines(...); */
	row_stride = image_width * 3; // JSAMPLEs per row in image_buffer

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// Step 6: Finish compression

	jpeg_finish_compress(&cinfo);

	fclose(outfile);

	// Step 7: release JPEG compression object

	jpeg_destroy_compress(&cinfo);
}

void im_read(string const & fname, Blob & data)
{
	Image im{fname};
	im.write(&data, "RGB");
}
