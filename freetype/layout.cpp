// render slova zo spravnym layoutom
#include <string>
#include <iostream>
#include <cassert>
#include <Magick++.h>
#include <ft2build.h>
#include FT_FREETYPE_H

std::string const word = "Teresa Lisbon";

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
	int const size = 16;
	err = FT_Set_Char_Size(face, size << 6, size << 6, 300, 300);
	assert(!err && "freetype set size failed");

	// save result
	using namespace Magick;
	Image im{Geometry{600, 300}, "black"};

	unsigned pen_x = 0, pen_y = 100;

	for (auto ch : word)
	{
		// load glyph
		unsigned char_code = ch;
		FT_UInt glyph_index = FT_Get_Char_Index(face, char_code);
		err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);  // now we can access face->glyph
		assert(!err && "can't load a glyph");

		// render glyph to bitmap
		FT_GlyphSlot glyph = face->glyph;
		err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
		assert(!err && "unable to render a glyph");
		FT_Bitmap & bitmap = glyph->bitmap;

		std::cout << ch << ", width:" << bitmap.width << ", height:" << bitmap.rows << "\n";

		// copy to result im
		PixelPacket * pixels = im.getPixels(pen_x + glyph->bitmap_left, pen_y - glyph->bitmap_top, bitmap.width, bitmap.rows);
		unsigned char * buff = bitmap.buffer;
		for (int i = 0; i < (bitmap.width * bitmap.rows); ++i, ++pixels, ++buff)
			*pixels = ColorGray{*buff/255.0};
		im.syncPixels();

		pen_x += glyph->advance.x >> 6;  // >> 6 means / 64 (conversion from integer pixel)
	}

	im.write("out/layout.png");

	return 0;
}


