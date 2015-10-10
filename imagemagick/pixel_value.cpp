// hodnoty pixelou
#include <Magick++.h>

using namespace Magick;

int main(int argc, char * argv[])
{
	Image im{"assets/bitmaps/colorbar.png"};  // [0, 10, 20, 30, ..., 90] as rgb values

	for (int x = 0; x < 10; ++x)
	{
		Color c = im.pixelColor(x, 0);  // adresuje sa zhora, zdola ?
		Quantum r = c.redQuantum();
		Quantum g = c.greenQuantum();
		Quantum b = c.blueQuantum();
		ColorRGB rgb = im.pixelColor(x, 0);
		double dr = rgb.red();
		double dg = rgb.green();
		double db = rgb.blue();
		uint8_t ur = (dr * 255) + 0.5;
		uint8_t ug = (dg * 255) + 0.5;
		uint8_t ub = (db * 255) + 0.5;		
		uint8_t _ur = c.redQuantum() * (255.0/QuantumRange) + 0.5;  // x * (255/QuantumRange)
	}

	return 0;
}
