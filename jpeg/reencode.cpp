#include <string>
#include <utility>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <jpeglib.h>
#include <boost/program_options.hpp>
#include <Magick++.h>

using std::string;
using std::cout;
using std::pair;
using std::make_pair;
using namespace Magick;
namespace po = boost::program_options;

// returns image geometry and blob image representation
static pair<size_t, size_t> im_read(string const & fname, Blob & data);

// reencode [options] INPUT OUTPUT

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

	// prepare input data
	Blob jpeg_data;
	pair<size_t, size_t> geometry = im_read(input_filename, jpeg_data);
	uint8_t * image_buffer = (uint8_t *)jpeg_data.data();
	size_t const & image_width = geometry.first;
	size_t const & image_height = geometry.second;

	jpeg_compress_struct cinfo;

	jpeg_error_mgr jerr;
	FILE * outfile;               // target file
	JSAMPROW row_pointer[1];      // pointer to JSAMPLE row[s]
	int row_stride;               // physical row width in image buffer

	// Step 1: allocate and initialize JPEG compression object
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	// Step 2: specify data destination (eg, a file)
	if ((outfile = fopen(output_filename.c_str(), "wb")) == NULL)
	{
		fprintf(stderr, "can't open %s\n", output_filename.c_str());
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

pair<size_t, size_t> im_read(string const & fname, Blob & data)
{
	Image im{fname};
	im.write(&data, "RGB");
	return make_pair(im.columns(), im.rows());
}
