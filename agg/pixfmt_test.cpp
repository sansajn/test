#include <iostream>
#include <cstdint>
#include <agg_rendering_buffer.h>
#include <agg_pixfmt_rgb.h>

using std::cout;

int main(int argc, char * argv[])
{
	uint8_t patt_pixels[3*1] = {0x01, 0x02, 0x3};  // 1x1 RGB map
	agg::rendering_buffer rb{patt_pixels, 1, 1, 1*3};
	agg::pixfmt_rgb24 pb{rb};

	agg::rgba8 p = pb.pixel(0, 0);
	cout << (int)p.r << " " << (int)p.g << " " << (int)p.b << "\n";

	return 0;
}
