/*! C API based audio "player" sample with `uridecodebin uri=INPUT! audioconvert ! autoaudiosink` pipeline and error handling.

Run it this way

	./audio_capi file:///home/ja/test.opus

*/
#include <iostream>
#include <gst/gst.h>

using std::cerr, std::cout;

struct player_pipeline {
	GstElement * pipeline,
		* source,
		* convert,
		* sink;
};

void pad_added_handler(GstElement * src, GstPad * new_pad, player_pipeline * data);

int main(int argc, char * argv[]) {
	if (argc < 2) {
		cerr << "audio_capi INPUT\n"
			<< "Error: INPUT parameter missing, exit\n";
		return 1;
	}

	gst_init(&argc, &argv);

	char const * input_arg = argv[1];

	player_pipeline data;
	data.source = gst_element_factory_make("uridecodebin", "source");
	data.convert = gst_element_factory_make("audioconvert", "convert");
	data.sink = gst_element_factory_make("autoaudiosink", "sink");

	data.pipeline = gst_pipeline_new("test-pipeline");
	if (!data.pipeline || !data.source || !data.convert || !data.sink) {
		cerr << "Not all elements could be created.\n";
		return 2;
	}

	// Building the pipeline. Note that we are NOT linking the source at this
	// point. We will do it later.
	gst_bin_add_many(GST_BIN(data.pipeline), data.source, data.convert, data.sink, nullptr);
	if (!gst_element_link(data.convert, data.sink)) {
		cerr << "Elements could not be linked.\n";
		gst_object_unref(data.pipeline);
		return 2;
	}

	// set the uri to play
	g_object_set(data.source, "uri", input_arg, nullptr);

	// connect to the pad-added signal
	g_signal_connect(data.source, "pad-added", G_CALLBACK(pad_added_handler), &data);

	// start playing
	GstStateChangeReturn ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		cerr << "Unable to set the pipeline to the playing state.\n";
		gst_object_unref(data.pipeline);
		return 2;
	}

	// listen to the bus
	bool terminate = false;
	GstBus * bus = gst_element_get_bus(data.pipeline);
	do {
		GstMessage * msg = gst_bus_timed_pop_filtered(bus,	(GstClockTime)GST_CLOCK_TIME_NONE,
			(GstMessageType)(GST_MESSAGE_STATE_CHANGED|GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

		// parse message
		if (msg) {
			switch (GST_MESSAGE_TYPE(msg)) {
				case GST_MESSAGE_ERROR: {
					GError * err;
					gchar * debug_info;
					gst_message_parse_error(msg, &err, &debug_info);
					cerr << "error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << "\n";
					cerr << "debug information: " << (debug_info ? debug_info : "none") << "\n";
					g_clear_error(&err);
					g_free(debug_info);
					terminate = true;
					break;
				}

				case GST_MESSAGE_EOS: {
					cerr << "End-Of-Stream reached.\n";
					terminate = true;
					break;
				}

				case GST_MESSAGE_STATE_CHANGED: {
					// we are only interested in state-changed messages from the pipeline
					if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data.pipeline)) {
						GstState old_state, new_state, pending_state;
						gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
						cout << "Pipeline state changed from " << gst_element_state_get_name(old_state) << " to " << gst_element_state_get_name(new_state) << "\n";
					}
					break;
				}

				default:
					cerr << "Unexpected message received.\n";
					break;
			}
		}

		gst_message_unref(msg);
	} while (!terminate);

	// clean-up
	gst_object_unref(bus);
	gst_element_set_state(data.pipeline, GST_STATE_NULL);
	gst_object_unref(data.pipeline);

	return 0;
}

// this function will be called by the pad-added signal
void pad_added_handler(GstElement * src, GstPad * new_pad, player_pipeline * data) {
	cout << "received new pad '" << GST_PAD_NAME(new_pad) << "' from '" << GST_ELEMENT_NAME(src) << "'\n";

	GstPad * sink_pad = gst_element_get_static_pad(data->convert, "sink");
	if (gst_pad_is_linked(sink_pad)) {
		cout << "  We are allready linked. Ignoring.\n";
		gst_object_unref(sink_pad);
		return;
	}

	// check the new pad's type
	GstCaps * new_pad_caps = gst_pad_query_caps(new_pad, nullptr);
	GstStructure * new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
	gchar const * new_pad_type = gst_structure_get_name(new_pad_struct);
	if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
		cerr << "  It has type '" << new_pad_type << "' which is not raw audio. Ignoring.\n";

		gst_caps_unref(new_pad_caps);
		gst_object_unref(sink_pad);
		return;
	}

	// atempt to link
	GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
	if (GST_PAD_LINK_FAILED(ret))
		cout << "  Type is '" << new_pad_type << "' but link failed.\n";
	else
		cout << "  Link succeeded (type '" << new_pad_type << "').\n";

	// clean-up
	gst_caps_unref(new_pad_caps);
	gst_object_unref(sink_pad);
}
