/*! C API based video "player" sample with `filesrc location=INPUT ! decodebin ! autovideosink` pipeline and error handling.

Run it this way

	./video_capi test.mkv

*/
#include <string>
#include <iostream>
#include <cassert>
#include <boost/format.hpp>
#include <gst/gst.h>

using std::string, std::to_string;
using std::cerr, std::cout, std::endl;
using boost::format, boost::str;

struct player_pipeline {
	GstElement * pipeline,
		* source,
		* decode,
		* sink;
};

void pad_added_handler(GstElement * src, GstPad * new_pad, player_pipeline * player);
string state_change_return_to_string(GstStateChangeReturn state);


int main(int argc, char * argv[]) {
	if (argc < 2) {
		cerr << "video_capi INPUT\n"
			<< "Error: INPUT parameter missing, exit\n";
		return 1;
	}

	gst_init(&argc, &argv);

	char const * input_arg = argv[1];

	// Our pipeline looks this way `filesrc ! decodebin ! autovideosink`
	string const pipeline_desc = str(format("filesrc location=%1% ! "
		"decodebin ! "
		"autovideosink") % input_arg);

	cout << "Pipeline: " << pipeline_desc << "\n";

	player_pipeline player;
	player.source = gst_element_factory_make("filesrc", "source");
	assert(player.source);

	player.decode = gst_element_factory_make("decodebin", "decode");
	assert(player.decode);

	player.sink = gst_element_factory_make("autovideosink", "sink");
	assert(player.sink);

	player.pipeline = gst_pipeline_new("player-pipeline");
	assert(player.pipeline);

	// Build the pipeline. Note we are not linking decode with sink at this point.
	gst_bin_add_many(GST_BIN(player.pipeline), player.source, player.decode, player.sink, nullptr);
	gboolean linked = gst_element_link_many(player.source, player.decode, nullptr);
	if (!linked) {
		cerr << "Source and decode elements could not be linked together.\n";
		gst_object_unref(player.pipeline);
		return 2;
	}

	// Connect to the *pad-added* signal
	g_signal_connect(player.decode, "pad-added", G_CALLBACK(pad_added_handler), &player);

	g_object_set(player.source, "location", input_arg, nullptr);  // set source element location

	GstStateChangeReturn changed = gst_element_set_state(player.pipeline, GST_STATE_PLAYING);  // start playing
	cout << "Changing pipeline state to PLAYING ... " << state_change_return_to_string(changed) << "\n";

	if (changed == GST_STATE_CHANGE_FAILURE) {
		cerr << "Unable to set the pipeline to the playing state.\n";
		gst_object_unref(player.pipeline);
		return 2;
	}

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(player.pipeline);
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
					if (GST_MESSAGE_SRC(msg) == GST_OBJECT(player.pipeline)) {
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
	gst_element_set_state(player.pipeline, GST_STATE_NULL);
	gst_object_unref(player.pipeline);

	cout << "done!\n";

	return 0;
}

void pad_added_handler(GstElement * src, GstPad * new_pad, player_pipeline * player) {
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
