// null value reading

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
	in << R"({"name":null, "test":{"foo":123}})";
	
	ptree json;
	read_json(in, json);
	string s = json.get<string>("name");
	cout << s << std::endl;  // will print 'null'
	
	ptree const & name_child = json.get_child("name");
	cout << "name_child.size():" << name_child.size() << "\n";
	
	ptree const & test_child = json.get_child("test");
	cout << "test_child.size():" << test_child.size() << "\n";

	return 0;
} 
