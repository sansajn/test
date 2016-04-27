#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

int main(int argc, char * argv[])
{
	pt::ptree json;
	pt::read_json("array.json", json);

	for (pt::ptree::value_type & number : json.get_child("numbers"))
	{
		string name = number
	}

	return 0;
}
