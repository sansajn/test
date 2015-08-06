// pristup k bloku pixelom obrazka
#include <Magick++.h>

int main(int argv, char * argc[])
{
	using namespace Magick;

	int const w = 30, h = 30;
	Image im{Geometry{w,h}, "white"};

	int const center_x = w/2, center_y = h/2;
	int const size = 10;

	im.modifyImage();  // reduce references to the underlaying image to one

	PixelPacket * pixels = im.getPixels(center_x-size/2, center_y-size/2, size, size);
	for (int i = 0; i < size*size; ++i, ++pixels)
		*pixels = ColorRGB{0.8, 0.5, 0};
	im.syncPixels();

	im.write("out/pixels.png");

	return 0;
}
