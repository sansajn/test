// manual hello world
#include <iostream>
#include <gst/gst.h>

using std::cerr;

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);

	GstElement * source = gst_element_factory_make("videotestsrc", "source");
	GstElement * sink = gst_element_factory_make("autovideosink", "sink");
	GstElement * pipeline = gst_pipeline_new("test_pipeline");

	if (!pipeline || !source || !sink)
	{
		cerr << "Not all elements could be created.\n";
		return -1;
	}

	// build the pipeline
	gst_bin_add_many(GST_BIN(pipeline), source, sink, nullptr);
	if (gst_element_link(source, sink) != TRUE)
	{
		cerr << "Elements could not be linked.\n";
		gst_object_unref(pipeline);
		return -1;
	}

	// modify the source properties
	g_object_set(source, "pattern", 0, nullptr);

	// start playing
	GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		cerr << "Unable to set the pipeline to the playing state./n";
		gst_object_unref(pipeline);
		return -1;
	}

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(pipeline);
	GstMessage * msg = gst_bus_timed_pop_filtered(
		bus, (GstClockTime)GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

	// parse message
	if (msg)
	{
		GError * err;
		gchar * debug_info;

		switch (GST_MESSAGE_TYPE(msg))
		{
			case GST_MESSAGE_ERROR:
				gst_message_parse_error(msg, &err, &debug_info);
				cerr << "error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << "\n";
				cerr << "debug information: " << (debug_info ? debug_info : "none") << "\n";
				g_clear_error(&err);
				g_free(debug_info);
				break;

			case GST_MESSAGE_EOS:
				cerr << "End-Of-Stream reached.\n";
				break;

			default:
				cerr << "Unexpected message received.\n";
				break;
		}
	}

	// clean-up
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	return 0;
}
