// seeking example
#include <iostream>
#include <gst/gst.h>

using std::cerr;
using std::cout;

struct CustomData
{
	GstElement * playbin;
	gboolean playing;
	gboolean terminate;
	gboolean seek_enabled;
	gboolean seek_done;
	gint64 duration;
};

void handle_message(CustomData * data, GstMessage * msg);

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);

	CustomData data;
	data.playing = FALSE;
	data.terminate = FALSE;
	data.seek_enabled = FALSE;
	data.seek_done = FALSE;
	data.duration = GST_CLOCK_TIME_NONE;

	data.playbin = gst_element_factory_make("playbin", "playbin");
	if (!data.playbin)
	{
		cerr << "Not all elements could be created.\n";
		return -1;
	}

	// set URI to play
	g_object_set(data.playbin,
		"uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", nullptr);

	// start playing
	GstStateChangeReturn ret = gst_element_set_state(data.playbin, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		cerr << "Unable to set the pipeline to the playing state./n";
		gst_object_unref(data.playbin);
		return -1;
	}

	// listen to the bus
	GstBus * bus = gst_element_get_bus(data.playbin);
	do
	{
		GstMessage * msg = gst_bus_timed_pop_filtered(bus,	100 * GST_MSECOND,
			(GstMessageType)(GST_MESSAGE_STATE_CHANGED|GST_MESSAGE_ERROR|GST_MESSAGE_EOS|GST_MESSAGE_DURATION));

		// parse message
		if (msg)
			handle_message(&data, msg);
		else
		{
			if (data.playing)
			{
				gint64 current = -1;

				// query the current position of the stream
				if (!gst_element_query_position(data.playbin, GST_FORMAT_TIME, &current))
					cerr << "Could not query current position.\n";

				// if we didn't know it yet, query the stream duration
				if (!GST_CLOCK_TIME_IS_VALID(data.duration))
					if (!gst_element_query_duration(data.playbin, GST_FORMAT_TIME, &data.duration))
						cerr << "Could not query current duration./n";

				g_print("Position %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
					GST_TIME_ARGS(current), GST_TIME_ARGS(data.duration));

				// if seeking is enabled, we have not done it yet, and the time is right, seek
				if (data.seek_enabled && !data.seek_done && current > 10 * GST_SECOND)
				{
					cout << "\nReached 10s, performing seek...\n";
					gst_element_seek_simple(data.playbin, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH|GST_SEEK_FLAG_KEY_UNIT), 30 * GST_SECOND);
					data.seek_done = TRUE;
				}
			}
		}
	}
	while (!data.terminate);

	// clean-up
	gst_object_unref(bus);
	gst_element_set_state(data.playbin, GST_STATE_NULL);
	gst_object_unref(data.playbin);
	return 0;
}

void handle_message(CustomData * data, GstMessage * msg)
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
			data->terminate = TRUE;
			break;

		case GST_MESSAGE_EOS:
			cerr << "End-Of-Stream reached.\n";
			data->terminate = TRUE;
			break;

		case GST_MESSAGE_DURATION:
			data->duration = GST_CLOCK_TIME_NONE;  // the duration has changed, mark the current one as invalid
			break;

		case GST_MESSAGE_STATE_CHANGED:
		{
			GstState old_state, new_state, pending_state;
			gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);

			if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data->playbin))
			{
				cout << "Pipeline state changed from " << gst_element_state_get_name(old_state) << " to " << gst_element_state_get_name(new_state) << "\n";

				data->playing = (new_state == GST_STATE_PLAYING);  // remember whether we are in the PLAING state or not
				if (data->playing)
				{
					// we just moved to PLAYING. Check if seeking is possible
					GstQuery * query = gst_query_new_seeking(GST_FORMAT_TIME);
					if (gst_element_query(data->playbin, query))
					{
						gint64 start, end;
						gst_query_parse_seeking(query, nullptr, &data->seek_enabled, &start, &end);
						if (data->seek_enabled)
							g_print("Seeking is ENABLED from %" GST_TIME_FORMAT " to %" GST_TIME_FORMAT "\n",
								GST_TIME_ARGS(start), GST_TIME_ARGS(end));
						else
							cout << "Seeking in DISABLED for this stream.\n";
					}
					else
						cerr << "Seeking query failed.\n";

					gst_query_unref(query);
				}
			}
			break;
		}

		default:
			cerr << "Unexpected message received.\n";
			break;
	}
}
