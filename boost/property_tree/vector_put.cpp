// vytvori pole prvkou v strome
#include <vector>
#include <boost/property_tree/json_parser.hpp>

using std::vector;
namespace pt = boost::property_tree;

int main(int argc, char * argv[]) 
{
	vector<int> arr{1, 2, 3, 4, 5};

	pt::ptree numbers_node;
	for (int number : arr)
	{
		pt::ptree local;
		local.put("", number);
		numbers_node.push_back(std::make_pair("", local));
	}

	pt::ptree root;
	root.add_child("numbers", numbers_node);

	pt::write_json("out_numbers.json", root);

	return 0;
}
