#include <iostream>
#include <Magick++.h>

using std::cout;

int main(int argc, char * argv[])
{
	using namespace Magick;

	Image im{"checker3x3.png"};
	Blob blob;
	im.write(&blob, "RGBA");
	cout << "size:" << blob.length() << "\n";

	uint8_t const * pixels = (uint8_t const *)blob.data();

	return 0;
}
