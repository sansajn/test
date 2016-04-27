// iteracia polom objektou
#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::string;
using std::stringstream;
namespace pt = boost::property_tree;

char const * json_data = R"(
{"data":[
	{"size":10, "color":"white"},
	{"size":15, "color":"black"}
]}
)";

int main(int argc, char * argv[])
{
	stringstream ss{json_data};
	pt::ptree json;
	pt::read_json(ss, json);

	for (pt::ptree::value_type & obj : json.get_child("data"))
	{
		pt::ptree const & ch = obj.second;
		cout << "(" << ch.get<int>("size") << ", " << ch.get<string>("color") << ")\n";
	}

	return 0;
}
