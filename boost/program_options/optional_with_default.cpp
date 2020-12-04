/* two optional options --encoder NAME (with h264 as default), --page PAGE
usage: ./optional_with_default [--encoder h264|vp8][--page PATH] */
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

using std::cout, std::string;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
	string arg_encoder;

	po::options_description desc{"commandline options"};
	desc.add_options()
		("help", "produce help messages")
		("encoder", po::value<string>(&arg_encoder)->default_value("h264"), "video encoder")
		("page", po::value<string>(), "use external html page to show video content");

	// process arguments
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help"))
	{
		cout << desc << "\n";
		return 1;
	}

	if (vm.count("page"))
		cout << "external page defined: " << vm["page"].as<string>() << "\n";
	else
		cout << "page not defined, default page used\n";

	cout << "encoder: " << arg_encoder << "\n";


	cout << "done!\n";
	return 0;
}
