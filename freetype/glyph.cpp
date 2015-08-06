// vyrenderuje glyph
#include <iostream>
#include <cassert>
#include <Magick++.h>
#include <ft2build.h>
#include FT_FREETYPE_H

int main(int argc, char * argv[])
{
	FT_Library ftlib;
	FT_Error err = FT_Init_FreeType(&ftlib);
	assert(!err && "free-type initialization failed");

	FT_Face face;
	err = FT_New_Face(ftlib,
		"/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf", 0, &face);
	assert(!err && "can't load font face");

	// set proper font size
	int const size = 32;
	err = FT_Set_Char_Size(face, size << 6, size << 6, 300, 300);
	assert(!err && "freetype set size failed");

	// load glyph
	unsigned char_code = 65;  // A
	FT_UInt glyph_index = FT_Get_Char_Index(face, char_code);
	err = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);  // now we can access face->glyph
	assert(!err && "can't load a glyph");

	// render glyph to bitmap
	err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	assert(!err && "unable to render a glyph");
	FT_Bitmap & bitmap = face->glyph->bitmap;

	// save result
	Magick::Image im;
	im.read(bitmap.width, bitmap.rows, "I", Magick::StorageType::CharPixel, bitmap.buffer);
	im.write("out/glyph.png");

	return 0;
}

