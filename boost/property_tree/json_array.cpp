// how to parse json array
#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::string;
using std::stringstream;
using namespace boost::property_tree;

string array_json = R"({"numbers" : ["one", "two", "three"]})";


int main(int argc, char * argv[])
{
	ptree json;
	stringstream ss{array_json};
	read_json(ss, json);

	// for loop iteration
	for (ptree::value_type & number : json.get_child("numbers"))
	{
		string value = number.second.data();
		cout << value << "\n";
	}

	ptree const & numbers = json.get_child("numbers");
	cout << "numbers.size()=" << numbers.size() << "\n";  // 3
	cout << "numbers.data()=" << numbers.data() << "\n";  // ""
	cout << "numbers.get_value()=" << numbers.get_value<string>() << "\n";  // ""

	cout << "elements: [";
	for (ptree::value_type const & n : numbers)  // n=(key: string, value: ptree)
		cout << n.second.data() << ", ";
	cout << "]\n";

	return 0;
}

