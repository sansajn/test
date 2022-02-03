// Processing bus events as signals in GLib loop sample.
#include <iostream>
#include <cassert>
#include <gst/gst.h>

using std::cout, std::cerr;

gboolean async_bus_handler(GstBus * bus, GstMessage * msg, gpointer data);

struct environment {
	GstElement * pipeline;
};

int main(int argc, char * argv[]) {
	gst_init(&argc, &argv);

	GstElement * pipeline = gst_parse_launch(
		"videotestsrc ! videoconvert ! autovideosink", nullptr);
	assert(pipeline);

	GstBus * bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	assert(bus);

	environment env;
	env.pipeline = pipeline;

	gst_bus_add_watch_full(bus, G_PRIORITY_HIGH, (GstBusFunc)async_bus_handler, 
		static_cast<gpointer>(&env), nullptr);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);  // start playing

	GMainLoop * loop = g_main_loop_new(nullptr, FALSE);
	g_main_loop_run(loop);  // blocking

	// clean-up
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);

	g_main_loop_unref(loop);

	gst_deinit();

	return 0;
}

gboolean async_bus_handler(GstBus * bus, GstMessage * msg, gpointer data) {
	assert(data);

	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_ERROR: {
			GError * err = nullptr;
			gchar * debug_info = nullptr;
			gst_message_parse_error(msg, &err, &debug_info);
			
			cerr << "Error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << "\n";
			cerr << "Debug information: " << (debug_info ? debug_info : "none") << "\n";
			
			g_clear_error(&err);
			g_free(debug_info);
			
			break;
		}

		case GST_MESSAGE_EOS: {
			cout << "EOS (End-Of-Stream) reached.\n";
			break;
		}

		case GST_MESSAGE_STATE_CHANGED: {
			environment const & env = *static_cast<environment *>(data);

			// we are only interested in state-changed messages from the pipeline
			if (GST_MESSAGE_SRC(msg) == GST_OBJECT(env.pipeline)) {
				GstState old_state, new_state, pending_state;
				gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
				cout << "Pipeline state changed from " << gst_element_state_get_name(old_state) << " to " << gst_element_state_get_name(new_state) << ".\n";
			}
			break;
		}

		default: break;
	}
	return TRUE;
}
