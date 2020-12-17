/*! video encoder like commandline processing sample
venc [--encoder h264|vp8, =h264][--verbose][--log PATH][--output,o PATH] [--input]INPUT */

#include <string>
#include <iostream>
#include <boost/program_options.hpp>

using std::cout, std::endl, std::string, std::exception;
namespace po = boost::program_options;

string const DEFAULT_ENCODER = "h264";


int main(int argc, char * argv[])
{
	string opt_encoder;

	po::options_description opts;
	opts.add_options()
		("help", "produce help messages")
		("encoder", po::value<string>(&opt_encoder)->default_value(DEFAULT_ENCODER),  // option with default value
			"used video encoder, available encoders are h264, h265, vp8 or vp9")
		("verbose", po::bool_switch()->default_value(false), "verbose output")  // switch option
		("log", po::value<string>(), "create log file")  // string value option
		("output,o", po::value<string>()->required(), "save converted video as arg");  // required option

	// options not shown to the user
	po::options_description hidden_opts;
	hidden_opts.add_options()
		("input", po::value<string>()->required(), "input video file");

	po::positional_options_description pos_opts;  // positional options
	pos_opts.add("input", 1);

	po::options_description cmd_opts;
	cmd_opts.add(opts).add(hidden_opts);

	po::options_description visible_opts{"Options"};  // to generate help message
	visible_opts.add(opts);

	po::variables_map args;

	// process arguments
	po::store(po::command_line_parser{argc, argv}
		.options(cmd_opts)
		.positional(pos_opts)
		.run(), args);

	try {
		po::notify(args);
	}
	catch (exception & e)
	{
		if (!args.count("help"))
			cout << "error: " << e.what() << "\n\n";

		cout << "Usage: venc [OPTION]... -o OUTPUT [--input]INPUT\n"
			<< "\n"
			<< visible_opts << "\n";

		return 1;
	}

	cout << "encoder=" << opt_encoder << "\n"
		<< "log=" << (args.count("log") ? args["log"].as<string>() : "not used") << "\n"
		<< "verbose=" << std::boolalpha << args["verbose"].as<bool>() << "\n"
		<< "output=" << args["output"].as<string>() << "\n"
		<< "input=" << args["input"].as<string>() << "\n";

	cout << "done!\n";
	return 0;
}
