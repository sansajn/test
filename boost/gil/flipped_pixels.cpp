#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test2x4rgb.png";

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgb8_image_t im;
	png_read_image(image_path, im);
	rgb8_view_t v = view(im);

	cout << "normal:\n";
	uint8_t * pixels = (uint8_t *)&(*v.begin());
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			int r = *pixels++;
			int g = *pixels++;
			int b = *pixels++;
			cout << "(" << r << "," << g << "," << b << "), ";
		}
		cout << "\n";
	}

	cout << "\nflipped:\n";
	rgb8_view_t flipped_v = flipped_up_down_view(view(im));
	pixels = (uint8_t *)&(*flipped_v.begin());
	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			int r = *pixels++;
			int g = *pixels++;
			int b = *pixels++;
			cout << "(" << r << "," << g << "," << b << "), ";
		}
		cout << "\n";
	}

	cout << "\nflipped, iterator approach:\n";
	for (rgb8_view_t::iterator it = flipped_v.begin(); it != flipped_v.end(); ++it)
		cout << "(" << (int)at_c<0>(*it) << "," << (int)at_c<1>(*it) << "," << (int)at_c<2>(*it) << ")\n";

	return 0;
}
