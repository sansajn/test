#include <string>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csetjmp>
#include <jpeglib.h>
#include <boost/program_options.hpp>

using std::string;
using std::cout;
using std::pair;
using std::make_pair;
namespace po = boost::program_options;

// reencode [options] INPUT OUTPUT

uint8_t * read_image(string const & filename, size_t & w, size_t & h);
void write_image(uint8_t * pixels, size_t w, size_t h, int q, string const & filename);


int main(int argc, char * argv[])
{
	po::options_description desc{"options"};
	desc.add_options()
		("help", "how to use this program")
		("quality,q", po::value<int>(), "set compression quality 0 - 100 where 100 is the best quality");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << desc << std::endl;
		return 1;
	}

	int quality = 80;
	if (vm.count("quality"))
		quality = vm["quality"].as<int>();

	string input_filename{"lena.jpg"};
	string output_filename{"out.jpg"};
	if (argc > 2 && argv[argc-2][0] != '-' && argv[argc-1][0] != '-')
	{
		input_filename = argv[argc-2];
		output_filename = argv[argc-1];
	}

	size_t w, h;
	uint8_t * pixels = read_image(input_filename, w, h);

	write_image(pixels, w, h, quality, output_filename);

	cout << "done!\n";
}


uint8_t * read_image(string const & filename, size_t & w, size_t & h)
{
	FILE * fin = fopen(filename.c_str(), "rb");
	assert(fin);

	jpeg_decompress_struct cinfo;

	// error handling
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);

	// read jpeg
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fin);
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
	fclose(fin);
	delete [] rows;
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return pixels;
}

void write_image(uint8_t * pixels, size_t w, size_t h, int q, string const & filename)
{
	jpeg_compress_struct cinfo;

	jpeg_error_mgr jerr;
	FILE * outfile;               // target file
	JSAMPROW row_pointer[1];      // pointer to JSAMPLE row[s]
	int row_stride;               // physical row width in image buffer

	// Step 1: allocate and initialize JPEG compression object
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	// Step 2: specify data destination (eg, a file)
	if ((outfile = fopen(filename.c_str(), "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", filename.c_str());
		exit(1);
	}

	jpeg_stdio_dest(&cinfo, outfile);

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

	fclose(outfile);

	// Step 7: release JPEG compression object

	jpeg_destroy_compress(&cinfo);
}
