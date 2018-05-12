// polygony
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <iostream>
#include <osmium/handler.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>

using std::vector;
using std::list;
using std::map;
using std::move;
using std::cout;

using polygon_type = vector<osmium::Location>;

class polygon_handler_impl : public osmium::handler::Handler
{
public:
	using ways = list<vector<size_t>>;
	using node_map = map<size_t, osmium::Location>;

	size_t way_count = 0;

	polygon_handler_impl(vector<polygon_type> & polys)
		: _polys{polys}
	{}

	void apply(osmium::io::Reader & osm);

	void way(osmium::Way & w);
	void node(osmium::Node & n);

private:
	ways _ways;
	node_map _nodes;
	vector<polygon_type> & _polys;
};

void polygon_handler_impl::apply(osmium::io::Reader & osm)
{
	_polys.clear();
	_polys.shrink_to_fit();

	osmium::apply(osm, *this);

	_polys.reserve(_ways.size());

	while (!_ways.empty())
	{
		polygon_type p;
		for (size_t nid : _ways.front())
		{
			osmium::Location & loc = _nodes[nid];
			if (loc.valid())
				p.push_back(loc);
		}

		assert(p.size() > 1);
		_polys.push_back(p);

		_ways.pop_front();
	}

	_nodes.clear();
}

void polygon_handler_impl::way(osmium::Way & w)
{
	++way_count;

	if (w.nodes().is_closed())
	{
		vector<size_t> tmp;
		for (osmium::NodeRef const & n : w.nodes())
			tmp.push_back(n.ref());
		_ways.push_back(move(tmp));
	}
}

void polygon_handler_impl::node(osmium::Node & n)
{
	_nodes[n.id()] = n.location();
}


int main(int argc, char * argv[])
{
	vector<polygon_type> polys;

	osmium::io::Reader osm{"u_uranie.osm"};
	polygon_handler_impl h{polys};
	h.apply(osm);
	osm.close();

	cout << "ways:" << h.way_count << "\n";
	cout << "polygons:" << polys.size() << "\n";

	return 0;
}
