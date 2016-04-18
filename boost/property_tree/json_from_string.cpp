#include <string>
#include <sstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using std::string;
using std::cout;
namespace pt = boost::property_tree;


template <typename Ptree>
void read_json_mem(string const & s, Ptree & pt)
{
	std::stringstream ss{s};
	boost::property_tree::read_json(ss, pt);
}


int main(int argc, char * argv[])
{
	string s{R"({"Cmd":"SetPosition", "lat":"48.145238", "lon":"17.111319"})"};
	pt::ptree root;
	read_json_mem(s, root);

	string cmd = root.get<string>("Cmd");
	double lat = root.get<double>("lat");
	double lon = root.get<double>("lon");

	cout << "Cmd:" << cmd << "\n"
		<< "lat:" << lat << ", lon:" << lon << std::endl;

	return 0;
}
