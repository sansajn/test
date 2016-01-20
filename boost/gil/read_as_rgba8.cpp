#include <iostream>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "test1x4rgb.png";

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgba8_image_t im;
	png_read_and_convert_image(image_path, im);

	for (auto & p : view(im))
		cout << "(" << (int)at_c<0>(p) << "," << (int)at_c<1>(p) << "," << (int)at_c<2>(p) << "," << (int)at_c<3>(p) << ")\n";

	return 0;
}
