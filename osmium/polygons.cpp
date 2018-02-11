// polygony
#include <vector>
#include <iostream>
#include <osmium/handler.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>

using std::cout;
using std::vector;

using polygon_type = vector<osmium::Location>;

class polygon_handler_impl : public osmium::handler::Handler
{
public:
	size_t way_count = 0;

	polygon_handler_impl(vector<polygon_type> & polys)
		: _polys{polys}
	{}

	void way(osmium::Way & w) {
		++way_count;

		if (w.nodes().is_closed())
		{
			polygon_type p;
			for (osmium::NodeRef const & n : w.nodes())
				p.push_back(n.location());
			_polys.push_back(p);
		}
	}

private:
	vector<polygon_type> _polys;
};

int main(int argc, char * argv[])
{
	vector<polygon_type> polys;

	osmium::io::Reader reader{"u_uranie.osm"};
	polygon_handler_impl h{polys};
	osmium::apply(reader, h);
	reader.close();

	cout << "ways:" << h.way_count << "\n";
	cout << "polygons:" << polys.size() << "\n";

	return 0;
}
