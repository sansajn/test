// pristup k jednotlivim pixelom obrazka skrz pixelColor
#include <Magick++.h>

int main(int argc, char * argv[])
{
	using namespace Magick;

	int const w = 300, h = 300;
	Image im(Geometry{w, h}, ColorRGB{1, 1, 1});

	int const center_x = w/2, center_y = h/2;
	int const radius = w/3;

	for (int y = center_y - radius - 1; y < center_y + radius + 1; ++y)
	{
		for (int x = center_x - radius - 1; x < center_x + radius + 1; ++x)
		{
			int dx = x - center_x;
			int dy = y - center_y;
			if ((dx*dx + dy*dy) <= radius*radius)
				im.pixelColor(x, y, ColorRGB{0.5, 0.8, 0});
		}
	}

	im.write("out/pixcol.png");

	return 0;
}
