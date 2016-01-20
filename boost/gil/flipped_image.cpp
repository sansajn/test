#include <iostream>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test2x4rgb.png";

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgba8_image_t im;
	png_read_and_convert_image(image_path, im);
	rgba8_image_t flipped{im.dimensions()};
	copy_pixels(flipped_up_down_view(view(im)), view(flipped));
	uint8_t * pixels = (uint8_t *)&(*view(flipped).begin());
	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			int r = *pixels++;
			int g = *pixels++;
			int b = *pixels++;
			int a = *pixels++;
			cout << "(" << r << "," << g << "," << b << "," << a << ")\n";
		}
	}
	return 0;
}
