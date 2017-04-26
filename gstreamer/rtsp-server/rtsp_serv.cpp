// rtsp server s testovacim videom
#include <string>
#include <iostream>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

using std::string;
using std::cout;

string server_address(GstRTSPServer * serv);
string server_service(GstRTSPServer * serv);  // returns server port


int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);
	
	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);

	// rtsp server
	GstRTSPServer * server = gst_rtsp_server_new();
	gst_rtsp_server_set_address(server, "192.168.88.142");
	gst_rtsp_server_set_service(server, "9554");

	// content pipeline
	GstRTSPMediaFactory * factory = gst_rtsp_media_factory_new();
	gst_rtsp_media_factory_set_launch(factory,
		"( videotestsrc is-live=1 ! x264enc ! rtph264pay name=pay0 pt=96 )");
	
	gst_rtsp_media_factory_set_shared(factory, TRUE);  // share pipeline between clients

	// attach specific url
	GstRTSPMountPoints * mounts = gst_rtsp_server_get_mount_points(server);  // get default mount points from the server
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
