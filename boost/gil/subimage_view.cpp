#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "out_subimaged.png";

using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgb8_image_t im{50,50};
	fill_pixels(view(im), rgb8_pixel_t{255,0,0});
	rgb8_image_t::view_t sub_view = subimage_view(view(im), 10, 10, 30, 30);
	fill_pixels(sub_view, rgb8_pixel_t{0,0,255});
	png_write_view(image_path, view(im));
	return 0;
}
