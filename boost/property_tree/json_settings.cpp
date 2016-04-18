#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::string;
using std::cout;
namespace pt = boost::property_tree;

int main(int argc, char * argv[])
{
	pt::ptree root;
	pt::read_json("test.json", root);
	// throw an exception if error (json_parser::json_parser_error)

	int height = root.get<int>("height", 0);
	string rabit_color = root.get<string>("animals.rabbit", "pink");

	cout << "height:" << height << "\n"
		<< "animals.rabbit:" << rabit_color << std::endl;

	try {  // throwing get() version
		string bat_color = root.get<string>("animals.bat");
	}
	catch (pt::ptree_error & e) {cout << e.what() << std::endl;}

	// update ptree
	root.put("animals.bat", "black");

	pt::write_json("test_out.json", root);

	return 0;
}
