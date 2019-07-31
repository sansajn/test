// how to parse json array
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/property_tree/json_parser.hpp>

using std::cout;
using std::string;
using std::stringstream;
using std::vector;
using namespace boost::property_tree;

string array_json = R"({
	"label": "some simple numbers",
	"numbers": ["one", "two", "three"],
	"dictionary": {"one": 1, "two": 2, "three": 3}
})";


template <typename T>
void get(ptree const & root, string const & key, vector<T> & result)
{
	ptree const & arr = root.get_child(key);
	if (!arr.data().empty())
		throw ptree_error{"unable to parse array object, reason '" + arr.data() + "' is not an array"};

	result.reserve(arr.size());
	for (ptree::value_type const & keyval : arr)
		result.push_back(keyval.second.get_value<T>());
}


int main(int argc, char * argv[])
{
	ptree json;
	stringstream ss{array_json};
	read_json(ss, json);

	string label = json.get<string>("label");

	ptree const & label_child = json.get_child("label");
	cout << "label.size()=" << label_child.size() << "\n";

	ptree const & dict_child = json.get_child("dictionary");
	cout << "dictionary.size()=" << dict_child.size() << "\n";

	ptree const & numbers = json.get_child("numbers");
	cout << "label=" << label << "\n";
	cout << "numbers=[";
	for (ptree::value_type const & n : numbers)
		cout << n.second.get_value<string>() << ", ";
	cout << "]\n";

	vector<string> v;
	get(json, "label", v);
	cout << "numbers:[";
	for (string const & elem : v)
		cout << elem << ", ";
	cout << "]\n";

	return 0;
}

