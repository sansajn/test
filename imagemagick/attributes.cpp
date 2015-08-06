// vytvaranie obrazka pomocou image-magick
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <Magick++.h>

using std::string;
using std::to_string;
using std::cout;
using std::min;

string type(Magick::Image const & im);
string density(Magick::Image const & im);

int main(int argc, char * argv[])
{
	using namespace Magick;

	Image im{Geometry{3, 1}, "white"};  // sRGB
	im.depth(8);

	cout << "image "
		<< "w:" << im.columns()
		<< ", h:" << im.rows()
		<< ", type:" << type(im)
		<< ", depth:" << im.depth()
		<< ", density:" << density(im)
		<< ", magick:" << im.magick()
		<< "\n";

//	PixelPacket * pixels = im.getPixels(0, 0, 3, 1);
//	for (int i = 0; i < (im.columns() * im.rows()); ++i, ++pixels)
//		*pixels = ColorGray{0.5};
//	im.syncPixels();

	Blob blob;
	im.write(&blob, "GRAY");

	cout << "blob " << "length:" << blob.length() << "\n";

	cout << "data: ";
	uint8_t const * data = (uint8_t const *)blob.data();
	for (int i = 0; i < min(10u, blob.length()); ++i)
		cout << std::hex << int(data[i]) << " ";
	cout << "\n";

	return 0;
}

string density(Magick::Image const & im)
{
	using namespace Magick;

	Geometry d = im.density();
	return to_string(d.width()) + "x" + to_string(d.height());
}

string type(Magick::Image const & im)
{
	using namespace Magick;

	ColorspaceType type = im.colorSpace();

	switch (type)
	{
		case sRGBColorspace: return string("sRGB");
		default: return std::to_string(type);
	}
}
