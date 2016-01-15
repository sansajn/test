#include <iostream>
#include <boost/gil/gil_all.hpp>

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	uint8_t * pixels = new uint8_t[5*4];
	rgba8_view_t im = interleaved_view(5, 1, (rgba8_pixel_t *)pixels, 5*4);
	im(0,0) = rgba8_pixel_t{0,0,0,0};
	im(1,0) = rgba8_pixel_t{1,1,1,1};
	im(2,0) = rgba8_pixel_t{2,2,2,2};
	im(3,0) = rgba8_pixel_t{3,3,3,3};
	im(4,0) = rgba8_pixel_t{4,4,4,4};
	
	cout << "pixels:";
	for (int i = 0; i < 5; ++i)
	{
		rgba8_pixel_t & p = im(i,0);
		cout << "(" << (int)at_c<0>(p) << "," << (int)at_c<1>(p) << "," << (int)at_c<2>(p) << "), ";
	}
	cout << "\n";

	cout << "size:" << im.size() << "\n";

	gray8_view_t::point_t p = im.dimensions();
	cout << "dimensions:" << p.x << "," << p.y << "\n";

	delete [] pixels;
	
	return 0;
}
