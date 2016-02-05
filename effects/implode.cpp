// implode to center effect (ekvivalent s resize)
#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/lexical_cast.hpp>
#include <glm/vec2.hpp>
#include <glm/common.hpp>

using std::cout;
using glm::vec2;
using glm::mix;
using boost::lexical_cast;
using namespace boost::gil;

int main(int argc, char * argv[])
{
	float implode_fact = 1.f;  // 1.f means no change [0, 1)
	if (argc > 1)
		implode_fact = lexical_cast<float>(argv[1]);

	char const * file_name;
	if (argc > 2)
		file_name = argv[2];
	else
		file_name = "ball_of_fire.png";

	rgb8_image_t im_src;
	png_read_image(file_name, im_src);

	int w = im_src.width(), h = im_src.height();
	rgb8_image_t im_dst{w, h};  // TODO: napln ciernou
	rgb8_view_t v_dst = view(im_dst);

	vec2 center = vec2{w/2.f, h/2.f};
	rgb8_view_t v_src = view(im_src);

	for (int y = 0; y < h; ++y)  // TODO: iteruj riadky
	{
		for (int x = 0; x < w; ++x)
		{
			rgb8_pixel_t p = v_src(x,y);
			vec2 st = mix(center, vec2{x,y}, implode_fact);  // compute new x,y coordinate (interpolate to center)
			v_dst(st.x, st.y) = p;
		}
	}

	png_write_view("out.png", v_dst);

	cout << "done.\n";
	return 0;
}
