// iteracia objektom
#include <string>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::string;
namespace pt = boost::property_tree;


int main(int argc, char * argv[])
{
	pt::ptree json;
	pt::read_json("test.json", json);

	for (pt::ptree::value_type & animal : json.get_child("animals"))
	{
		string name = animal.first;
		string color = animal.second.data();
		cout << name << ":" << color << "\n";
	}

	return 0;
}
