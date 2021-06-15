/*! C API based PAD capabilities setup sample with `capsfilter` element.

Based on following pipeline:

	videotestsrc !
		video/x-raw, width=1280, height=720 !
	videoconvert !
	autovideosink

Run it this way

	./video_caps

*/
#include <string>
#include <iostream>
#include <cassert>
#include <boost/format.hpp>
#include <gst/gst.h>

using std::string, std::to_string;
using std::cerr, std::cout, std::endl;
using boost::format, boost::str;

struct caps_pipeline {
	GstElement * pipeline,
		* source,
		* source_caps,
		* convert,
		* sink;
};

string state_change_return_to_string(GstStateChangeReturn state);


int main(int argc, char * argv[]) {
	gst_init(&argc, &argv);

	// pipeline visualisation, just as hint
	string const pipeline_desc =
		"videotestsrc ! "
			"video/x-raw, width=1280, height=720 !"
		"videoconvert ! "
		"autovideosink";

	cout << "Pipeline: " << pipeline_desc << "\n";

	caps_pipeline pipe;
	pipe.source = gst_element_factory_make("videotestsrc", "source");
	assert(pipe.source);

	pipe.source_caps = gst_element_factory_make("capsfilter", "source_caps");
	assert(pipe.source);

	pipe.convert = gst_element_factory_make("videoconvert", "convert");
	assert(pipe.convert);

	pipe.sink = gst_element_factory_make("autovideosink", "sink");
	assert(pipe.sink);

	pipe.pipeline = gst_pipeline_new("player-pipeline");
	assert(pipe.pipeline);

	// Build the pipeline.
	gst_bin_add_many(GST_BIN(pipe.pipeline),
		pipe.source, pipe.source_caps, pipe.convert, pipe.sink, nullptr);

	gboolean linked = gst_element_link_many(pipe.source, pipe.source_caps,
		pipe.convert, pipe.sink, nullptr);

	if (!linked) {
		cerr << "Pipeline can not be linked together.\n";
		gst_object_unref(pipe.pipeline);
		return 2;
	}

	// set source capabilities
	GstCaps * caps = gst_caps_new_simple("video/x-raw",
		"width", G_TYPE_INT, 1280,
		"height", G_TYPE_INT, 720, nullptr);

	g_object_set(pipe.source_caps, "caps", caps, nullptr);

	gst_caps_unref(caps);  // we do not need caps anymore

	GstStateChangeReturn changed = gst_element_set_state(pipe.pipeline, GST_STATE_PLAYING);  // start playing
	cout << "Changing pipeline state to PLAYING ... " << state_change_return_to_string(changed) << "\n";

	if (changed == GST_STATE_CHANGE_FAILURE) {
		cerr << "Unable to set the pipeline to the playing state.\n";
		gst_object_unref(pipe.pipeline);
		return 2;
	}

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(pipe.pipeline);
	assert(bus);

	bool terminate = false;
	do {
		GstMessage * msg = gst_bus_timed_pop_filtered(
			bus, (GstClockTime)GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_STATE_CHANGED|GST_MESSAGE_ERROR|GST_MESSAGE_EOS));

		if (msg) {
			switch (GST_MESSAGE_TYPE(msg)) {
				case GST_MESSAGE_EOS: {
					cout << "end-of-stream reached" << endl;
					terminate = true;
					break;
				}

				case GST_MESSAGE_STATE_CHANGED: {
					if (GST_MESSAGE_SRC(msg) == GST_OBJECT(pipe.pipeline)) {
						GstState old_state, new_state, pending_state;
						gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
						cout << "Pipeline changed from " << gst_element_state_get_name(old_state) << " to "
							<< gst_element_state_get_name(new_state) << endl;
					}
					break;
				}

				case GST_MESSAGE_ERROR: {
					GError * err;
					gchar * dinfo;
					gst_message_parse_error(msg, &err, &dinfo);
					cerr << "Error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << "\n"
						<< "Debugging information: " << (dinfo ? dinfo : "none") << endl;
					g_clear_error(&err);
					g_free(dinfo);
					terminate = true;
					break;
				}

				default:
					cerr << "unexpected message\n";
					break;
			}

			gst_message_unref(msg);
		}
	} while (!terminate);

	// clean-up
	gst_object_unref(bus);
	gst_element_set_state(pipe.pipeline, GST_STATE_NULL);
	gst_object_unref(pipe.pipeline);

	cout << "done!\n";

	return 0;
}

void pad_added_handler(GstElement * src, GstPad * new_pad, caps_pipeline * player) {
	cout << "Received new pad '" << GST_PAD_NAME(new_pad) << "' from '" << GST_ELEMENT_NAME(src) << "':\n";

	assert(player);
	GstPad * sink_pad = gst_element_get_static_pad(player->sink, "sink");

	if (gst_pad_is_linked(sink_pad)) {
		cout << "We are already linked. Ignoring.\n";
		gst_object_unref(sink_pad);
		return;
	}

	// check new pad's type
	GstCaps * new_pad_caps = gst_pad_get_current_caps(new_pad);
	GstStructure * new_pad_structure = gst_caps_get_structure(new_pad_caps, 0);  // get the first caps structure
	gchar const * new_pad_type = gst_structure_get_name(new_pad_structure);
	if (!g_str_has_prefix(new_pad_type, "video/x-raw")) {
		cout << "It has type '" << new_pad_type << "' which is not raw video. Ignoring.\n";
		gst_caps_unref(new_pad_caps);
		gst_object_unref(sink_pad);
		return;
	}

	// attempt to link
	GstPadLinkReturn linked = gst_pad_link(new_pad, sink_pad);
	if (GST_PAD_LINK_FAILED(linked))
		cerr << "Type is '" << new_pad_type << "' but link failed.\n";
	else
		cout << "Link succeeded (type '" << new_pad_type << "').\n";

	// clean-up
	gst_caps_unref(new_pad_caps);
	gst_object_unref(sink_pad);
}

string state_change_return_to_string(GstStateChangeReturn state) {
	switch (state) {
		case GST_STATE_CHANGE_FAILURE: return "STATE_CHANGE_FAILURE";
		case GST_STATE_CHANGE_SUCCESS: return "STATE_CHANGE_SUCCESS";
		case GST_STATE_CHANGE_ASYNC: return "STATE_CHANGE_ASYNC";
		case GST_STATE_CHANGE_NO_PREROLL: return "STATE_CHANGE_NO_PREROLL";
		default: return "UNKNOWN_STATE_CHANGE";
	}
}
