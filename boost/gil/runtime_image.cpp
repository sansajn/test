// precita obrazky rozneho typu (poctu kanalov)
#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <boost/gil/extension/dynamic_image/any_image_view.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>

char const * image_path = "test1x4rgba.png";

using std::cout;
using namespace boost::gil;

using image_t = boost::mpl::vector<gray8_image_t, rgb8_image_t, rgba8_image_t>;

int main(int argc, char * argv[])
{
	any_image<image_t> im;
	png_read_image(image_path, im);
	
	any_image<image_t>::view_t v = view(im);
//	any_image<image_t>::view_t::iterator it = v.begin(), it_end = v.end();
	
	// toto je problematicke, koklo mam kanalov ?
//	for (; it != it_end; ++it)
//		cout << "(" << at_c<0>(*it) << "," << at_c<1>(*it) << "," << at_c<2>(*it) << ")\n";
	
	return 0;
}
