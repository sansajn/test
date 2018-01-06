// spocita uzly v osm subore
#include <string>
#include <iostream>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

using std::string;
using std::cout;

string const default_osm_input = "atm.osm";

struct count_handler : public osmium::handler::Handler
{
	uint64_t count = 0;
	void node(osmium::Node const & n);
	void tag_list(osmium::TagList const & tags);
};

void count_handler::node(osmium::Node const & n)
{
	++count;

	osmium::Location const & loc = n.location();
	cout << "(" << loc.lat() << ", " << loc.lon() << ")";

	// also show tag-name
	osmium::TagList const & tags = n.tags();
	char const * val = tags["name"];
	if (val)
		cout << ", " << val;

	cout << "\n";

//	for (auto const & t : n.tags())
//		cout << "key:" << t.key() << ", value:" << t.value() << "\n";
}

void count_handler::tag_list(osmium::TagList const & tags)
{
	cout << "tag_list\n";
}


int main(int argc, char * argv[])
{
	string in = argc > 1 ? argv[1] : default_osm_input;

	osmium::io::File fin{in};
	osmium::io::Reader reader{fin};

	count_handler node_counter;
	osmium::apply(reader, node_counter);

	reader.close();

	cout << "nodes:" << node_counter.count << "\n"
		<< "done.\n";

	return 0;
}

