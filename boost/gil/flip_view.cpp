#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

char const * image_path = "girl.png";

using namespace boost::gil;

int main(int argc, char * argv[])
{
	rgb8_image_t im;
	png_read_image(image_path, im);
	rgb8_image_t::view_t flipped_v = flipped_up_down_view(view(im));
	png_write_view("out_flipped.png", flipped_v);
	return 0;
}
