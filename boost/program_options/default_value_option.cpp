int main(int argc, char * argv[])
{
	string arg_pls;

	po::options_description desc{"commandline options"};
	desc.add_options()
		("help", "produce help messages")
		("image-fps", po::value<int>(), "image frame rate")
		("pls", po::value<string>(&arg_pls)->default_value("assets/test.pls"),
			"playlist file");

	po::positional_options_description pos_desc;
	pos_desc.add("pls", 1);  // make pls positional option

	// process arguments
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
	po::notify(vm);
	
	// ...
}