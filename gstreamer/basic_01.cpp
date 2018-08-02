// hello world
#include <gst/gst.h>

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);
	GstElement * pipeline = gst_parse_launch(
		"playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", nullptr);
	gst_element_set_state(pipeline, GST_STATE_PLAYING);  // start playing

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(pipeline);
	GstMessage * msg = gst_bus_timed_pop_filtered(
		bus, (GstClockTime)GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

	// clean-up
	if (msg)
		gst_message_unref(msg);

	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
