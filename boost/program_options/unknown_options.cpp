// allows for unknown options
#include <iostream>
#include <boost/program_options.hpp>

using std::cout;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
	po::options_description desc{"Allowed options"};
	desc.add_options()
		("help", "produce help messages")
		("compression", po::value<int>(), "set complession level");

	po::variables_map vm;
	po::parsed_options parsed = po::basic_command_line_parser<char>(argc, argv).options(desc).allow_unregistered().run();
	po::store(parsed, vm);
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

	return 0;
}
