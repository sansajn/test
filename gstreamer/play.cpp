// play locally stored file file
#include <string>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <gst/gst.h>

using std::string;
using std::cerr;

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cerr << "input file missing, exit\n";
		return -1;
	}

	string ifile = argv[1];

	gst_init(&argc, &argv);

	GstElement * playbin = gst_element_factory_make("playbin", "playbin");
	assert(playbin && "unable to create a playbin element");

	char buf[1024];
	getcwd(buf, sizeof(buf));
	string uri = "file://" + string{buf} + string{"/"} + ifile;
	g_object_set(playbin, "uri", uri.c_str(), nullptr);

	// start playing
	GstStateChangeReturn ret = gst_element_set_state(playbin, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		cerr << "Unable to set the pipeline to the playing state.\n";
		gst_object_unref(playbin);
		return -1;
	}

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(playbin);
	GstMessage * msg = gst_bus_timed_pop_filtered(bus, (GstClockTime)GST_CLOCK_TIME_NONE,
		(GstMessageType)(GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

	// clean-up
	if (msg)
		gst_message_unref(msg);

	gst_object_unref(bus);
	gst_element_set_state(playbin, GST_STATE_NULL);
	gst_object_unref(playbin);

	return 0;
}
