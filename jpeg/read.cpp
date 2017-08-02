// citanie jpeg suboru
#include <cstdio>
#include <csetjmp>
#include <cassert>
#include <jpeglib.h>

char const * image_path = "lena.jpg";

struct my_error_mgr {
	jpeg_error_mgr pub;  // public fields ...
	jmp_buf setjmp_buffer;
};

void my_error_exit(j_common_ptr cinfo)
{
	my_error_mgr * myerr = (my_error_mgr *)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer, 1);  // return control tothe setjmp point
}

int main(int argc, char * argv[])
{
	FILE * fin = fopen(image_path, "rb");
	assert(fin);
	
	// step 1, decompression object
	jpeg_decompress_struct cinfo;  // preco cinfo ?
	
	my_error_mgr jerr;  // error handling, precitaj v dokumentacii
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	if (setjmp(jerr.setjmp_buffer))
	{
		assert(0 && "jpeg error");
		return 1;
	}
	
	jpeg_create_decompress(&cinfo);  // jpeg decompression object
	
	// step 2, source
	jpeg_stdio_src(&cinfo, fin);
	
	// step 3, file parameters
	jpeg_read_header(&cinfo, TRUE);
	
	// step 4, decompression parameters
	
	// step 5, start decompressor
	jpeg_start_decompress(&cinfo);
	int row_stride = cinfo.output_width * cinfo.output_components;  // ekvivalent rowbytes
	JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);  // ekvivalent rows s png
	
	// step 6, reading pixels
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
//		put_scanline_someplace(buffer[0], row_stride);  // toto su akoze data, ktore som prave precital (riadok ?)
	}
	
	// step 7, finish decompression
	jpeg_finish_decompress(&cinfo);
	
	// step 8, cleanup
	jpeg_destroy_decompress(&cinfo);
	
	fclose(fin);
	
	return 0;
}
