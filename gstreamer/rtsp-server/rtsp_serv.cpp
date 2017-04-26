// rtsp server s testovacim videom
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <boost/program_options.hpp>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

using std::string;
using std::ostringstream;
using std::cout;
namespace po = boost::program_options;

string pwd();
string server_address(GstRTSPServer * serv);
string server_service(GstRTSPServer * serv);  // returns server port


int main(int argc, char * argv[])
{
	po::options_description desc{"rtsp-serv options"};
	desc.add_options()
		("help", "produce help message")
		("host", po::value<string>(), "ip4 streaming address (i.e. 127.0.0.1)")
		("port", po::value<string>(), "streaming server port (i.e. 8554")
		("if", po::value<string>(), "imput file");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << desc << std::endl;
		return 0;
	}

	string host_arg{"127.0.0.1"};
	if (vm.count("host"))
		host_arg = vm["host"].as<string>();

	string port_arg{"8554"};
	if (vm.count("port"))
		port_arg = vm["port"].as<string>();

	string if_arg;
	if (vm.count("if"))
		if_arg = vm["if"].as<string>();


	gst_init(&argc, &argv);
	
	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);
	assert(loop);

	// rtsp server
	GstRTSPServer * server = gst_rtsp_server_new();
	assert(server);
	gst_rtsp_server_set_address(server, host_arg.c_str());
	gst_rtsp_server_set_service(server, port_arg.c_str());

	// content pipeline
	GstRTSPMediaFactory * factory = gst_rtsp_media_factory_new();
	assert(factory);

	string launch;
	if (!if_arg.empty())
	{
		string ifile = pwd() + "/" + if_arg;

		ostringstream oss;
		oss << "( filesrc location=\"" << ifile << "\" "
			<< "! qtdemux name=d "
			<< "d. ! queue ! rtph264pay pt=96 name=pay0 "
			<< "d. ! queue ! rtpmp4apay pt=97 name=pay1 "
			<< " )";
		launch = oss.str();
	}
	else
		launch = "( videotestsrc is-live=1 ! x264enc ! rtph264pay name=pay0 pt=96 )";

	gst_rtsp_media_factory_set_launch(factory, launch.c_str());

	gst_rtsp_media_factory_set_shared(factory, TRUE);  // share pipeline between clients

	// attach specific url
	GstRTSPMountPoints * mounts = gst_rtsp_server_get_mount_points(server);  // get default mount points from the server
	assert(mounts);
	gst_rtsp_mount_points_add_factory(mounts, "/test", factory);  // attach video test signal to the "/test" URL
	g_object_unref(mounts);

	gst_rtsp_server_attach(server, NULL);  // attach the server to the default maincontext

	string address = string{"rtsp://"} + server_address(server) + ":" + server_service(server) + "/test";
	cout << "stream ready at " << address << " use\n"
		<< "  $ ffplay " << address << "\n"
		<< "command to play it." << std::endl;
		
	g_main_loop_run(loop);  // start serving

	return 0;
}

string pwd()
{
	string result;
	size_t size = 1024;
	char * path = nullptr;
	do
	{
		char * buf = new char[size];
		path = getcwd(buf, size);
		if (path)
			result.assign(buf);
		delete [] buf;
		size *= 2;
	}
	while (!path && size < 64*1024);

	return result;
}

string server_address(GstRTSPServer * serv)
{
	gchar * address = gst_rtsp_server_get_address(serv);
	string result{address};
	g_free(address);
	return result;
}

string server_service(GstRTSPServer * serv)
{
	gchar * service = gst_rtsp_server_get_service(serv);
	string result{service};
	g_free(service);
	return result;
}
