// len vytvori obrazok 400x300 s bielim background-om
#include <Magick++.h>

using namespace Magick;

int main(int argc, char * argv[])
{
	Image im(Geometry(400, 300), ColorRGB(1.0, 1.0, 1.0));
	im.write("out/hello.png");
	return 0;
}
