// precita obrazok ako rgba 8-bit
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <png.h>

char const * file_name = "test2.png";


int main(int argc, char * argv[])
{
	FILE * fp = fopen(file_name, "rb");
	assert(fp);

	png_struct * png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	assert(png);

	png_info * info = png_create_info_struct(png);
	assert(info);

	if (setjmp(png_jmpbuf(png)))  // error handler
	{
		assert(0 && "png error");
		return 1;
	}

	png_init_io(png, fp);
	png_read_info(png, info);
	
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_method;
	png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, 
		&interlace_method, int_p_NULL, int_p_NULL);
	
	png_set_strip_16(png);  // strip 16 bit to 8 bit
	assert(bit_depth <= 8 && "16 bit depth image will be stripped to 8 bit");
	
	png_set_packing(png);  // convert 1, 2 and 4 bit to 8 bit
	
	if (color_type == PNG_COLOR_TYPE_PALETTE)  // convert paletted images to rgb
		png_set_palette_to_rgb(png);
	
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)  // expand gray images to 8 bit
		png_set_expand_gray_1_2_4_to_8(png);
	
	if (png_get_valid(png, info, PNG_INFO_tRNS))  // transparency to full alpha channel
		png_set_tRNS_to_alpha(png);
	
	if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)  // add alpha channel
		png_set_add_alpha(png, 0xff, PNG_FILLER_AFTER);
	
	png_read_update_info(png, info);
	
	unsigned rowbytes = png_get_rowbytes(png, info);

	uint8_t * pixels = new uint8_t[rowbytes * height];  // result pixels

	png_byte ** rows = new png_byte * [height];
	for (png_uint_32 r = 0; r < height; ++r)
		rows[r] = pixels + r*rowbytes;
	
	png_read_image(png, rows);	
	png_read_end(png, info);

	png_set_rows(png, info, rows);
	
	std::ostringstream oss;
	oss << "w:" << png_get_image_width(png, info) << ", h:" << png_get_image_height(png, info)
		<< ", ch:" << (int)png_get_channels(png, info) << ", pixel-depth:" << (int)info->pixel_depth
		<< ", bit-depth:" << (int)png_get_bit_depth(png, info)
		<< ", rowbytes:" << (int)png_get_rowbytes(png, info)
		<< ", valid:" << (int)info->valid;

	std::cout << oss.str() << std::endl;

	// pristup k datam
	png_byte ** __rows = png_get_rows(png, info);

	for (int y = 0; y < png_get_image_height(png, info); ++y)
	{
		png_byte * data = __rows[y];
		for (int x = 0; x < png_get_image_width(png, info); ++x)
		{
			std::cout << "(";
			for (int z = 0; z < png_get_channels(png, info); ++z)
				std::cout << (int)*data++ << " ";
			std::cout << "), ";
		}
		std::cout << std::endl;
	}

	// data mam aj v pixels ...

	png_destroy_read_struct(&png, &info, png_infopp_NULL);
	fclose(fp);

	delete [] rows;
	delete [] pixels;

	return 0;
}
