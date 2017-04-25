// rtsp server s testovacim videom
#include <iostream>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

using std::cout;

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);
	
	GMainLoop * loop = g_main_loop_new(NULL, FALSE);

	// rtsp server
	GstRTSPServer * server = gst_rtsp_server_new();

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

	cout << "stream ready at rtsp://127.0.0.1:8554/test, use\n" 
		<< "  $ ffplay rtsp://127.0.0.1:8554/test\n"
		<< "command to see it" << std::endl;
		
	g_main_loop_run(loop);  // start serving

	return 0;
}
