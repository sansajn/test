// precita obrazok obratene hore nohami
#include <iostream>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test2x4rgb.png";

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgba8_image_t::point_t d = png_read_dimensions(image_path);
	rgba8_image_t im{d};
	png_read_and_convert_view(image_path, flipped_up_down_view(view(im)));

	rgba8_view_t v = view(im);
	rgba8_view_t::iterator it = v.begin();
	for (int y = 0; y < v.height(); ++y)
	{
		for (int x = 0; x < v.width(); ++x)
		{
			cout << "(" << (int)at_c<0>(*it) << "," << (int)at_c<1>(*it) << "," << (int)at_c<2>(*it) << "," << (int)at_c<3>(*it) << "), ";
			++it;
		}
		cout << "\n";
	}

	return 0;
}
