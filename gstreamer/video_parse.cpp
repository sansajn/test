// `gst_parse_launch()` based video "player" sample with error handling
#include <string>
#include <iostream>
#include <cassert>
#include <boost/format.hpp>
#include <gst/gst.h>

using std::string, std::to_string;
using std::cerr, std::cout, std::endl;
using boost::format, boost::str;

string state_change_return_to_string(GstStateChangeReturn state) {
	switch (state) {
		case GST_STATE_CHANGE_FAILURE: return "STATE_CHANGE_FAILURE";
		case GST_STATE_CHANGE_SUCCESS: return "STATE_CHANGE_SUCCESS";
		case GST_STATE_CHANGE_ASYNC: return "STATE_CHANGE_ASYNC";
		case GST_STATE_CHANGE_NO_PREROLL: return "STATE_CHANGE_NO_PREROLL";
		default: return "UNKNOWN_STATE_CHANGE";
	}
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		cerr << "video_parse INPUT\n"
			<< "Error: INPUT parameter missing, exit\n";
		return 1;
	}

	gst_init(&argc, &argv);

	string pipeline_desc = str(format("filesrc location=%1% ! "
		"decodebin ! "
		"autovideosink") % argv[1]);

	cout << "Pipeline: " << pipeline_desc << "\n";

	GError * error = nullptr;
	GstElement * pipeline = gst_parse_launch(pipeline_desc.c_str(), &error);
	if (error)
	{
		cerr << "Could not create WebRTC pipeline: " << error->message << "\n";
		g_error_free(error);
		return 2;
	}
	assert(pipeline);

	GstStateChangeReturn changed = gst_element_set_state(pipeline, GST_STATE_PLAYING);  // start playing
	cout << "Changing pipeline state to PLAYING ... " << state_change_return_to_string(changed) << "\n";

	// wait until error or EOS
	GstBus * bus = gst_element_get_bus(pipeline);
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
					if (GST_MESSAGE_SRC(msg) == GST_OBJECT(pipeline)) {
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
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	cout << "done!\n";

	return 0;
}
