/* Sample to use flipped_up_down_view() view. 
for a documentation [Image View Transformation](https://www.boost.org/doc/libs/1_83_0/libs/gil/doc/html/reference/group___image_view_transformations.html) page.
Tested with Boost 1.83 */
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>

char const * image_path = "girl.png";

using namespace boost::gil;

int main(int argc, char * argv[]) {
	rgb8_image_t im;
	read_image(image_path, im, png_tag{});
	rgb8_image_t::view_t flipped_v = flipped_up_down_view(view(im));
	write_view("out_flipped.png", flipped_v, png_tag{});
	return 0;
}
