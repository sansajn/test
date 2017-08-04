#include <string>
#include <iostream>
#include <cstdio>
#include <csetjmp>
#include <cassert>
#include <jpeglib.h>

using std::cout;
using std::string;

struct error_mgr_impl
{
	jpeg_error_mgr pub;  // public fields ...
	jmp_buf setjmp_buffer;
};

void custom_error_exit(j_common_ptr info)
{
	error_mgr_impl * err = (error_mgr_impl *)info->err;
	(*info->err->output_message)(info);
	longjmp(err->setjmp_buffer, 1);  // return control to the setjmp point
}

static string to_string(J_COLOR_SPACE space);

int main(int argc, char * argv[])
{
	jpeg_decompress_struct dinfo;

	// error handling
	error_mgr_impl jerr;
	jerr.pub.error_exit = custom_error_exit;
	dinfo.err = jpeg_std_error(&jerr.pub);

	if (setjmp(jerr.setjmp_buffer))  // setjmp control point
	{
		assert(0 && "jpeg error");
		return 1;
	}

	jpeg_create_decompress(&dinfo);  // jpeg decompression state

	string input_file_name{"lena.jpg"};
	if (argc > 1)
		input_file_name = argv[1];

	FILE * fin = fopen(input_file_name.c_str(), "rb");
	assert(fin);

	jpeg_stdio_src(&dinfo, fin);  // source

	int result = jpeg_read_header(&dinfo, TRUE);
	assert(result == JPEG_HEADER_OK && "something wrong");

	// show header data
	cout << input_file_name << "\n"
		<< "w:" << dinfo.image_width << ", h:" << dinfo.image_height << "\n"
		<< "components: " << dinfo.num_components	<< "\n"
		<< "color-space: " << to_string(dinfo.jpeg_color_space) << "\n"
		<< "JFIF-marker:" << dinfo.saw_JFIF_marker << "\n";

	long pos = ftell(fin);
	cout << "offset: " << pos << " bytes\n";


	jvirt_barray_ptr * raw_dct_coeff = jpeg_read_coefficients(&dinfo);

	for (int i = 0; i < dinfo.num_components; ++i)
	{
		jpeg_component_info & comp_info = dinfo.comp_info[i];
		cout << "component #" << i << ". info:\n"
			<< "  component_id:" << comp_info.component_id << "\n"
			<< "  component_index (in SOF):" << comp_info.component_index << "\n"
			<< "  h_samp_factor:" << comp_info.h_samp_factor << "\n"
			<< "  v_samp_factor:" << comp_info.v_samp_factor << "\n"
			<< "  width_in_blocks:" << comp_info.width_in_blocks << "\n"
			<< "  height_in_blocks:" << comp_info.height_in_blocks << "\n"
			<< "  DCT_h_scaled_size:" << comp_info.DCT_h_scaled_size << "\n"
			<< "  DCT_v_scaled_size:" << comp_info.DCT_v_scaled_size << "\n"
			<< "  downsampled_width:" << comp_info.downsampled_width << "\n"
			<< "  downsampled_height:" << comp_info.downsampled_height << "\n"
			<< "  component_needed:" << comp_info.component_needed << "\n"
			<< "  MCU_width:" << comp_info.MCU_width << "\n"
			<< "  MCU_height:" << comp_info.MCU_height<< "\n"
			<< "  MCU_sample_width:" << comp_info.MCU_sample_width << "\n\n";
	}

	jpeg_destroy_decompress(&dinfo);

	fclose(fin);

	return 0;
}

string to_string(J_COLOR_SPACE cs)
{
	switch (cs)
	{
		case JCS_UNKNOWN: return "JCS_UNKNOWN";
		case JCS_GRAYSCALE: return "JCS_GRAYSCALE";
		case JCS_RGB: return "JCS_RGB";
		case JCS_YCbCr: return "JCS_YCbCr";
		case JCS_CMYK: return "JCS_CMYK";
		case JCS_YCCK: return "JCS_YCCK";
		case JCS_EXT_RGB: return "JCS_EXT_RGB";
		case JCS_EXT_RGBX: return "JCS_EXT_RGBX";
		case JCS_EXT_BGR: return "JCS_EXT_BGR";
		case JCS_EXT_BGRX: return "JCS_EXT_BGRX";
		case JCS_EXT_XBGR: return "JCS_EXT_XBGR";
		case JCS_EXT_XRGB: return "JCS_EXT_XRGB";
		case JCS_EXT_RGBA: return "JCS_EXT_RGBA";
		case JCS_EXT_BGRA: return "JCS_EXT_BGRA";
		case JCS_EXT_ABGR: return "JCS_EXT_ABGR";
		case JCS_EXT_ARGB: return "JCS_EXT_ARGB";
		default:
			throw std::logic_error{"unknown JPEG color spec"};
	}
}
