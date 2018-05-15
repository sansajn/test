#include <osmium/io/any_input.hpp>

int main(int argc, char * argv[])
{
	osmium::io::Reader osm{argc > 1 ? argv[1] : "u_uranie.osm"};
	return 0;
}
