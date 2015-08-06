// vyrenderuje slovo
#include <string>
#include <iostream>
#include <cassert>
#include <Magick++.h>
#include <ft2build.h>
#include FT_FREETYPE_H

std::string const word = "Ahoj!";

int main(int argc, char * argv[])
{
	FT_Library ftlib;
	FT_Error err = FT_Init_FreeType(&ftlib);
	assert(!err && "freetype initialization failed");

	FT_Face face;
	err = FT_New_Face(ftlib,
		"/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf", 0, &face);
	assert(!err && "can't load font face");

	// set proper font size
	int const size = 24;
	err = FT_Set_Char_Size(face, size << 6, size << 6, 300, 300);
	assert(!err && "freetype set size failed");

	// save result
	using namespace Magick;
	Image im{Geometry{600, 300}, ColorRGB{1,1,1}};

	unsigned left = 0;

	for (auto ch : word)
	{
		// load glyph
		unsigned char_code = ch;
		FT_UInt glyph_index = FT_Get_Char_Index(face, char_code);
		err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);  // now we can access face->glyph
		assert(!err && "can't load a glyph");

		// render glyph to bitmap
		err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		assert(!err && "unable to render a glyph");
		FT_Bitmap & bitmap = face->glyph->bitmap;

		std::cout << ch << ", width:" << bitmap.width << ", height:" << bitmap.rows << "\n";

		// copy to result im
		PixelPacket * pixels = im.getPixels(left, 10, bitmap.width, bitmap.rows);
		unsigned char * buff = bitmap.buffer;
		for (int i = 0; i < (bitmap.width * bitmap.rows); ++i, ++pixels, ++buff)
			*pixels = ColorGray{*buff/255.0};
		im.syncPixels();

		left += bitmap.width;
	}

	im.write("out/word.png");

	return 0;
}

