#include <iostream>
#include <boost/gil/gil_all.hpp>

using std::cout;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	uint8_t * pixels = new uint8_t[5];
	gray8_view_t im = interleaved_view(5, 1, (gray8_pixel_t *)pixels, 5);
	im(0,0) = 0;
	im(1,0) = 1;
	im(2,0) = 2;
	im(3,0) = 3;
	im(4,0) = 4;

	cout << "pixels:";
	for (int i = 0; i < 5; ++i)
		cout << (int)im(i,0) << ", ";
	cout << "\n";

	cout << "size:" << im.size() << "\n";

	gray8_view_t::point_t p = im.dimensions();
	cout << "dimensions:" << p.x << "," << p.y << "\n";

	delete [] pixels;
	
	return 0;
}
