//! RTSP player sample based on `gst_parse_launch()`
#include <string>
#include <boost/format.hpp>
#include <cassert>
#include <gst/gst.h>

using std::string;
using boost::format, boost::str;

constexpr char const * DEFAULT_LOCATION = "rtsp://localhost:8554/stream";

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);  // Initialize GStreamer

	string location = DEFAULT_LOCATION;
	if (argc > 1)
		location = argv[1];

	string const pipeline_desc = str(format(
		"rtspsrc location=%1% ! decodebin ! videoconvert ! autovideosink") % location);

	// Build the pipeline
	GstElement * pipeline = gst_parse_launch(pipeline_desc.c_str(), NULL);
	assert(pipeline);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);  // Start playing

	// Wait until error or EOS
	GstBus * bus = gst_element_get_bus(pipeline);
	assert(bus);

	GstMessage * msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
		GstMessageType(GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

	// Free resources
	if (msg)
		gst_message_unref(msg);
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
