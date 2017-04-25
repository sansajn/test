// inicializuje serverm neposkytuje ziaden obsah

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);
	
	GstRTSPServer * server = gst_rtsp_server_new();
	
	gst_rtsp_server_attach(server, NULL);  // attach the server to the default maincontext

	GMainLoop * loop = g_main_loop_new(NULL, FALSE);  // make a mainloop for the default context
	g_main_loop_run(loop);  // start serving

	return 0;
}
