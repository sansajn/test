// list json suboru
#include <string>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

char const * default_json_path = "test.json";

using std::cout;
using std::string;
namespace pt = boost::property_tree;

void dump_tree(pt::ptree const & root, string const & ident = string{});

int main(int argc, char * argv[])
{
	string json_path{argc > 1 ? argv[1] : default_json_path};

	pt::ptree json;
	pt::read_json(json_path, json);
	dump_tree(json);

	return 0;
}

void dump_tree(pt::ptree const & root, string const & ident)
{
	for (pt::ptree::value_type const & e : root)
	{
		pt::ptree const & ch = e.second;
		cout << ident << e.first << ":" << ch.data() << "\n";
		dump_tree(ch, ident + " ");
	}
}
