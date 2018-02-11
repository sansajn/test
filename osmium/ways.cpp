// iteracia cestami
#include <iostream>
#include <osmium/handler.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>

using std::cout;

struct handler : public osmium::handler::Handler
{
	size_t count = 0;

	void way(osmium::Way & w) {
		++count;
		cout << "way:" << w.id() << "\n";
		for (osmium::Tag const & t : w.tags())
			cout << "  " << t.key() << "=" << t.value() << "\n";
	}
};

int main(int argc, char * argv[])
{
	osmium::io::Reader reader{"u_uranie.osm"};
	handler h;
	osmium::apply(reader, h);
	reader.close();
	cout << "ways:" << h.count << "\n";
	return 0;
}
