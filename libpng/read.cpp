// precita png subor pomocou hi-level api
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
	png_read_png(png, info, 0, png_voidp_NULL);

	// data su dostupne ako ukazatele na riadky (data nie su kontinualne)
	png_byte ** rows = png_get_rows(png, info);
	for (int i = 0; i < std::min(png_get_image_height(png, info), 10ul); ++i)
		std::cout << (intptr_t)rows[i] << std::endl;

	std::ostringstream oss;
	oss << "w:" << png_get_image_width(png, info) << ", h:" << png_get_image_height(png, info)
		<< ", ch:" << (int)png_get_channels(png, info) << ", pixel-depth:" << (int)info->pixel_depth
		<< ", bit-depth:" << (int)png_get_bit_depth(png, info)
		<< ", rowbytes:" << (int)png_get_rowbytes(png, info)
		<< ", valid:" << (int)info->valid;
	
	std::cout << oss.str() << std::endl;

	// pristup k datam
	for (int y = 0; y < png_get_image_height(png, info); ++y)
	{
		png_byte * data = rows[y];
		for (int x = 0; x < png_get_image_width(png, info); ++x)
		{
			unsigned val = 0;
			for (int z = 0; z < png_get_channels(png, info); ++z)
				val += *data++;
			val /= png_get_channels(png, info);

			std::cout << val/255.0f;
		}
		std::cout << std::endl;
	}

	png_destroy_read_struct(&png, &info, png_infopp_NULL);
	fclose(fp);

	return 0;
}
