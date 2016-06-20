// nepovinne hodnoty
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
namespace pt = boost::property_tree;

int main(int argc, char * argv[])
{
	boost::optional<int> height_key = root.get_optional("height");
	if (height_key)
		cout << "height: " << *height_key << std::endl;
	return 0;
}
