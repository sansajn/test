/* one positional option (url) sample 
usage:  positional_options [--compression] {url} */
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

using std::string;
using std::cout;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
	po::options_description desc{"Allowed options"};
	desc.add_options()
		("help", "produce help messages")
		("compression", po::value<int>(), "set complession level")
		("url", po::value<string>(), "startup page");

	po::positional_options_description pos_desc;
	pos_desc.add("url", 1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
	po::notify(vm);  // to je naco (zrejme nejake call-backy) ?

	if (vm.count("help"))
	{
		cout << desc << "\n";
		return 1;
	}

	if (vm.count("compression"))
		cout << "Compression level was set to " << vm["compression"].as<int>() << ".\n";
	else
		cout << "Compression leel was not set.\n";
	
	if (vm.count("url"))
		cout << "url set to '" << vm["url"].as<string>() << "\n";
	else
		cout << "url was not set.\n";

	return 0;
}
