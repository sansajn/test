#include <sstream>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <csetjmp>
#include <jpeglib.h>
#include <stdexcept>

using std::ostringstream;
using std::string;
using std::cout;
using std::endl;

namespace jpeg {

struct custom_err_mgr
{
	jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};
char last_error_message[JMSG_LENGTH_MAX];

void jpeg_error_handler(j_common_ptr info)
{
	custom_err_mgr * mgr = (custom_err_mgr*)info->err;
	(*(info->err->format_message))(info, last_error_message);
	longjmp(mgr->setjmp_buffer, 1);
}

bool check_subsampling_is_422(string const & file_name)
{
	// read JPEG header
	FILE * fin = fopen(file_name.c_str(), "rb");
	if (!fin)
		return false;

	jpeg_decompress_struct dinfo;
	jpeg_create_decompress(&dinfo);

	custom_err_mgr jerr;
	dinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = jpeg_error_handler;
	if (setjmp(jerr.setjmp_buffer))
	{
		ostringstream sout;
		sout << "Error while reading JPEG file '" << file_name << "', what: " 
			<< last_error_message << endl;
		fclose(fin);
		return false;
	}

	jpeg_stdio_src(&dinfo, fin);
	jpeg_read_header(&dinfo, TRUE);

	bool result =
		((dinfo.num_components == 3) && (dinfo.jpeg_color_space == JCS_YCbCr));

	if (result)
	{
		size_t c0_w = dinfo.comp_info[0].downsampled_width;
		size_t c1_w = dinfo.comp_info[1].downsampled_width;
		size_t c2_w = dinfo.comp_info[2].downsampled_width;

		size_t c0_h = dinfo.comp_info[0].downsampled_height;
		size_t c1_h = dinfo.comp_info[1].downsampled_height;
		size_t c2_h = dinfo.comp_info[2].downsampled_height;

		if (c0_w != c1_w)  // horizontal
			result = ((c0_w == 2*c1_w) && (c1_w == c2_w));
		else  // vertical
			result = ((c0_h == 2*c1_h) && (c1_h == c2_h));
	}

	jpeg_destroy_decompress(&dinfo);
	fclose(fin);

	return result;
}

}  // jpeg

int main(int argc, char * argv[])
{
	bool passed = jpeg::check_subsampling_is_422("wrong.jpg");
	cout << "image " << (passed ? "passer" : "is broken") << "\n"
		<< "done!\n";
	return 0;
}
