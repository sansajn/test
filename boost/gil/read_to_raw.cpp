// precita obrazok do pola bajtov
#include <iostream>
#include <memory>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test1x4rgb.png";

using std::cout;
using std::unique_ptr;
using namespace boost::gil;


int main(int argc, char * argv[])
{
	unique_ptr<uint8_t []> pixels{new uint8_t[4*1*4]};  // RGBA
	png_read_and_convert_view(image_path, interleaved_view(4, 1, (rgba8_pixel_t *)pixels.get(), 4*1*4));

	uint8_t * p = pixels.get();
	for (int i = 0; i < 4; ++i)
	{
		int r = *p++;
		int g = *p++;
		int b = *p++;
		int a = *p++;
		cout << "(" << r << "," << g << "," << b << "," << a << ")\n";
	}

	return 0;
}
