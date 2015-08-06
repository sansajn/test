// zisti pocet glyphou v pisme (jeho face)
#include <iostream>
#include <cassert>
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

	unsigned glyphs = face->num_glyphs;
	std::cout << "glyphs:" << glyphs << "\n";

	return 0;
}
