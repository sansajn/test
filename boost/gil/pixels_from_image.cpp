// data s image (vzhladom na sposob citania png_read_image je tato metoda koser)
#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test1x4rgb.png";

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgb8_image_t im;
	png_read_image(image_path, im);

	rgb8_view_t v = view(im);

	cout << "iterator approach\n";
	rgb8_view_t::iterator it = v.begin();
	for (; it != v.end(); ++it)
	{
		rgb8_pixel_t p = *it;
		cout << (int)at_c<0>(p) << "," << (int)at_c<1>(p) << "," << (int)at_c<2>(p) << "\n";
	}

	cout << "from begin() approach\n";
	it = v.begin();
	rgb8_pixel_t & p = *it;
	uint8_t * pixels = (uint8_t *)&p;
	for (int i = 0; i < 4; ++i)
	{
		int r = *pixels++;
		int g = *pixels++;
		int b = *pixels++;
		cout << r << "," << g << "," << b << "\n";
	}

	cout << "direct cast begin()\n";
	uint8_t * data = (uint8_t *)&(*v.begin());
	for (int i = 0; i < 4; ++i)
	{
		int r = *data++;
		int g = *data++;
		int b = *data++;
		cout << r << "," << g << "," << b << "\n";
	}

	return 0;
}
