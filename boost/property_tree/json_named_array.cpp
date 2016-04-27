// iteracia pomenovanym polom
#include <string>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::string;
namespace pt = boost::property_tree;


int main(int argc, char * argv[])
{
	pt::ptree json;
	pt::read_json("array.json", json);

	for (pt::ptree::value_type & number : json.get_child("numbers"))
	{
		string value = number.second.data();
		cout << value << "\n";
	}

	return 0;
}
