// video playback with GLib loop instead of `gst_bus_X` call sample

#include <cassert>
#include <gst/gst.h>

int main(int argc, char * argv[]) {
	gst_init(&argc, &argv);

	GstElement * pipeline = gst_parse_launch(
		"videotestsrc ! videoconvert ! autovideosink", nullptr);
	assert(pipeline);
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
