/*! C API based video pipeline with RTP payload (`rtph264pay`) element and output to a window sample.

Based on following pipeline:

	videotestsrc ! convert ! queue !
	x264enc ! queue !
	rtph264pay ! rtph264depay ! queue !
	decodebin ! autovideosink

Run it this way

	./video_payload

*/
#include <string>
#include <iostream>
#include <cassert>
#include <boost/format.hpp>
#include <gst/gst.h>

using std::string, std::to_string;
using std::cerr, std::cout, std::endl;
using boost::format, boost::str;

struct payload_pipeline {
	GstElement * pipeline,
		* source,
		* convert,
		* queue_enc,
		* encoder,
		* queue_pay,
		* payload,
		* depayload,
		* queue_dec,
		* decode,
		* sink;
};

void pad_added_handler(GstElement * src, GstPad * new_pad, payload_pipeline * player);
string state_change_return_to_string(GstStateChangeReturn state);


int main(int argc, char * argv[]) {
	gst_init(&argc, &argv);

	string const pipeline_desc = {
		"videotestsrc ! convert ! queue ! "
		"x264enc ! queue ! "
		"rtph264pay ! rtph264depay ! queue ! "
		"decodebin ! autovideosink"};

	cout << "Pipeline: " << pipeline_desc << "\n";

	payload_pipeline paypipe;
	paypipe.source = gst_element_factory_make("videotestsrc", "source");
	assert(paypipe.source);

	paypipe.convert = gst_element_factory_make("videoconvert", "convert");
	assert(paypipe.convert);

	paypipe.queue_enc = gst_element_factory_make("queue", "queue_enc");
	assert(paypipe.queue_enc);

	paypipe.encoder = gst_element_factory_make("x264enc", "encode");
	assert(paypipe.encoder);

	paypipe.queue_pay = gst_element_factory_make("queue", "queue_pay");
	assert(paypipe.queue_pay);

	paypipe.payload = gst_element_factory_make("rtph264pay", "payload");
	assert(paypipe.payload);

	paypipe.depayload = gst_element_factory_make("rtph264depay", "depayload");
	assert(paypipe.depayload);

	paypipe.queue_dec = gst_element_factory_make("queue", "queue_decode");
	assert(paypipe.queue_dec);

	paypipe.decode = gst_element_factory_make("decodebin", "decode");
	assert(paypipe.encoder);

	paypipe.sink = gst_element_factory_make("autovideosink", "sink");
	assert(paypipe.sink);

	paypipe.pipeline = gst_pipeline_new("encoder-pipeline");
	assert(paypipe.pipeline);

	// Build the pipeline except decodebin and autovideosink which are linked later.
	gst_bin_add_many(GST_BIN(paypipe.pipeline),
		paypipe.source,
		paypipe.convert,
		paypipe.queue_enc,
		paypipe.encoder,
		paypipe.queue_pay,
		paypipe.payload,
		paypipe.depayload,
		paypipe.queue_dec,
		paypipe.decode,
		paypipe.sink, nullptr);

	gboolean linked = gst_element_link_many(
		paypipe.source,
		paypipe.convert,
		paypipe.queue_enc,
		paypipe.encoder,
		paypipe.queue_pay,
		paypipe.payload,
		paypipe.depayload,
		paypipe.queue_dec,
		paypipe.decode, nullptr);

	if (!linked) {
		cerr << "Pipeline elements could not be linked together.\n";
		gst_object_unref(paypipe.pipeline);
		return 2;
	}

	// Connect to the *pad-added* signal for decode
	g_signal_connect(paypipe.decode, "pad-added", G_CALLBACK(pad_added_handler), &paypipe);

	// Set source properties.
	g_object_set(paypipe.source, "pattern", 18, nullptr);  // ball (18), see inspect
	g_object_set(paypipe.source, "is-live", TRUE, nullptr);

	GstStateChangeReturn changed = gst_element_set_state(paypipe.pipeline, GST_STATE_PLAYING);  // start playing
	cout << "Changing pipeline state to PLAYING ... " << state_change_return_to_string(changed) << "\n";

	if (changed == GST_STATE_CHANGE_FAILURE) {
		cerr << "Unable to set the pipeline to the playing state.\n";
		gst_object_unref(paypipe.pipeline);
		return 2;
	}

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(paypipe.pipeline);
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
					if (GST_MESSAGE_SRC(msg) == GST_OBJECT(paypipe.pipeline)) {
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
	gst_element_set_state(paypipe.pipeline, GST_STATE_NULL);
	gst_object_unref(paypipe.pipeline);

	cout << "done!\n";

	return 0;
}

void pad_added_handler(GstElement * src, GstPad * new_pad, payload_pipeline * player) {
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
