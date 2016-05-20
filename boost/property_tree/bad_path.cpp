#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::string;
using std::cout;
using std::stringstream;
using namespace boost::property_tree;


int main(int argc, char * argv[])
{
	stringstream in;
	in << R"({"name":"Patric"})";
	
	try {
		ptree json;
		read_json(in, json);
		string s = json.get<string>("Name.Of.The.Misterious.Node");
	}
	catch (ptree_error const & e) {
		cout << e.what() << std::endl;
	}
	return 0;
} 
