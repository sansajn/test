#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <Magick++.h>

using std::cout;
using namespace boost::gil;
using namespace Magick;

unsigned at(boost::gil::rgba8_pixel_t const & p)
{
	using boost::gil::at_c;
	return (at_c<0>(p) << 24)|(at_c<1>(p) << 16)|(at_c<2>(p) << 8)|at_c<3>(p);
}

unsigned at(Color const & c)
{
	unsigned result =
		unsigned(c.redQuantum() * (255.0/QuantumRange) + 0.5) << 24 |
		unsigned(c.greenQuantum() * (255.0/QuantumRange) + 0.5) << 16 |
		unsigned(c.blueQuantum() * (255.0/QuantumRange) + 0.5) << 8 |
		unsigned(c.alphaQuantum() * (255.0/QuantumRange) + 0.5);
	return result;
}

unsigned to_ui(short v)
{
	return v * 255.0/QuantumRange;
}

int main(int argc, char * argv[])
{
	Image magick_im{"test1x4rgb.png"};
	Blob pixels;
	magick_im.write(&pixels, "RGBA");

	unsigned w = magick_im.columns(), h = magick_im.rows();
	rgba8_view_t im = interleaved_view(w, h, (rgba8_pixel_t *)pixels.data(), w*4);

	for (unsigned y = 0; y < h; ++y)
		for (unsigned x = 0; x < w; ++x)
		{
			Color c = magick_im.pixelColor(x,y);
			rgba8_pixel_t & p = im(x,y);
			cout << "magick: ("
				<< to_ui(c.redQuantum()) << ", "
				<< to_ui(c.greenQuantum()) << ", "
				<< to_ui(c.blueQuantum()) << ", "
				<< to_ui(c.alphaQuantum())
				<< "), gil: ("
				<< (int)at_c<0>(p) << ", "
				<< (int)at_c<1>(p) << ", "
				<< (int)at_c<2>(p) << ", "
				<< (int)at_c<3>(p) << ")\n";
		}

	cout << "size:" << im.size() << "\n";

	gray8_view_t::point_t p = im.dimensions();
	cout << "dimensions:" << p.x << "," << p.y << "\n";

	return 0;
}
