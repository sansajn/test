// ako na GUI
#include <string>
#include <cstring>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include "pwd.hpp"

using std::string;

struct custom_data
{
	GstElement * playbin;
	GtkWidget * slider;
	GtkWidget * streams_list;
	gulong slider_update_signal_id;
	GstState state;
	gint64 duration;  //!< in ns
};

static void realize_cb(GtkWidget * widget, custom_data * data)
{
	GdkWindow * window = gtk_widget_get_window(widget);

	if (!gdk_window_ensure_native(window))
		g_error("Couldn't create native window needed for GstVideoOverlay!");

	guintptr window_handle = GDK_WINDOW_XID(window);
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(data->playbin), window_handle);
}

static void play_cb(GtkButton * button, custom_data * data)
{
	gst_element_set_state(data->playbin, GST_STATE_PLAYING);
}

static void pause_cb(GtkButton * button, custom_data * data)
{
	gst_element_set_state(data->playbin, GST_STATE_PAUSED);
}

static void stop_cb(GtkButton * button, custom_data * data)
{
	gst_element_set_state(data->playbin, GST_STATE_READY);
}

// called when the main window is closed
static void delete_event_cb(GtkWidget * widget, GdkEvent * event, custom_data * data)
{
	stop_cb(nullptr, data);
	gtk_main_quit();
}

static gboolean draw_cb(GtkWidget * widget, cairo_t * cr, custom_data * data)
{
	if (data->state < GST_STATE_PAUSED)
	{
		GtkAllocation allocation;
		gtk_widget_get_allocation(widget, &allocation);
		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_rectangle(cr, 0, 0, allocation.width, allocation.height);
		cairo_fill(cr);
	}
	return FALSE;
}

static void slider_cb(GtkRange * range, custom_data * data)
{
	gdouble value = gtk_range_get_value(GTK_RANGE(data->slider));
	gst_element_seek_simple(data->playbin, GST_FORMAT_TIME,
		(GstSeekFlags)(GST_SEEK_FLAG_FLUSH|GST_SEEK_FLAG_KEY_UNIT), (gint64)(value * GST_SECOND));
}

static void create_ui(custom_data * data)
{
	GtkWidget * main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(main_window), "delete-event", G_CALLBACK(delete_event_cb), data);

	GtkWidget * video_window = gtk_drawing_area_new();
	gtk_widget_set_double_buffered(video_window, FALSE);
	g_signal_connect(video_window, "realize", G_CALLBACK(realize_cb), data);
	g_signal_connect(video_window, "draw", G_CALLBACK(draw_cb), data);

	GtkWidget * play_button = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_SMALL_TOOLBAR);
	g_signal_connect(G_OBJECT(play_button), "clicked", G_CALLBACK(play_cb), data);

	GtkWidget * pause_button = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_SMALL_TOOLBAR);
	g_signal_connect(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_cb), data);

	GtkWidget * stop_button = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_SMALL_TOOLBAR);
	g_signal_connect(G_OBJECT(stop_button), "clicked", G_CALLBACK(stop_cb), data);

	data->slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
	gtk_scale_set_draw_value(GTK_SCALE(data->slider), 0);
	data->slider_update_signal_id = g_signal_connect(G_OBJECT(data->slider), "value-changed", G_CALLBACK(slider_cb), data);

	data->streams_list = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(data->streams_list), FALSE);

	GtkWidget * controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(controls), play_button, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls), pause_button, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls), stop_button, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(controls), data->slider, TRUE, TRUE, 2);

	GtkWidget * main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_hbox), video_window, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_hbox), data->streams_list, FALSE, FALSE, 2);

	GtkWidget * main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), main_hbox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(main_box), controls, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(main_window), main_box);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 640, 480);

	gtk_widget_show_all(main_window);
}

static gboolean refresh_ui(custom_data * data)
{
	if (data->state < GST_STATE_PAUSED)
		return TRUE;

	if (!GST_CLOCK_TIME_IS_VALID(data->duration))
	{
		if (!gst_element_query_duration(data->playbin, GST_FORMAT_TIME, &data->duration))
			g_printerr("Could not query current duration.\n");
		else
			gtk_range_set_range(GTK_RANGE(data->slider), 0, (gdouble)data->duration / GST_SECOND);
	}

	gint64 current = -1;
	if (gst_element_query_position(data->playbin, GST_FORMAT_TIME, &current))
	{
		// block the "value-changed" signal, so the slider_cb is not called
		// which would trigger a seek the user has not requested
		g_signal_handler_block(data->slider, data->slider_update_signal_id);
		gtk_range_set_value(GTK_RANGE(data->slider), (gdouble)current / GST_SECOND);
		g_signal_handler_unblock(data->slider, data->slider_update_signal_id);
	}

	return TRUE;
}

// called when metadata is discovered in the stream
static void tags_cb(GstElement * playbin, gint stream, custom_data * data)
{
	// we are possibly in a GStreamer working thread, so we notify the main
	// thread of this event through a message in the bus.
	gst_element_post_message(playbin,
		gst_message_new_application(GST_OBJECT(playbin),
			gst_structure_new_empty("tags-changed")));
}

static void error_cb(GstBus * bus, GstMessage * msg, custom_data * data)
{
	GError * err;
	gchar * debug_info;
	gst_message_parse_error(msg, &err, &debug_info);
	g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
	g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
	g_clear_error(&err);
	g_free(debug_info);

	gst_element_set_state(data->playbin, GST_STATE_READY);  // set the pipeline to READY (which stops playback)
}

static void eos_cb(GstBus * bus, GstMessage * msg, custom_data * data)
{
	g_print("End-Of-Stream reached.\n");
	gst_element_set_state(data->playbin, GST_STATE_READY);
}

static void state_changed_cb(GstBus * bus, GstMessage * msg, custom_data * data)
{
	GstState old_state, new_state, pending_state;
	gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
	if (GST_MESSAGE_SRC(msg) == GST_OBJECT(data->playbin))
	{
		data->state = new_state;
		g_print("State set to %s\n", gst_element_state_get_name(new_state));
		if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED)
			refresh_ui(data);
	}
}

// extract metadata from all the streams and write it to the text widget in the GUI
static void analyze_streams(custom_data * data)
{
	  guint rate;

	GtkTextBuffer * text = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->streams_list));
	gtk_text_buffer_set_text(text, "", -1);

	gint n_video, n_audio, n_text;
	g_object_get(data->playbin, "n-video", &n_video, nullptr);
	g_object_get(data->playbin, "n-audio", &n_audio, nullptr);
	g_object_get(data->playbin, "n-text", &n_text, nullptr);

	GstTagList * tags;
	gchar * str, * total_str;
	for (gint i = 0; i < n_video; ++i)
	{
		tags = nullptr;
		g_signal_emit_by_name(data->playbin, "get-video-tags", i, &tags);
		if (tags)
		{
			total_str = g_strdup_printf("video stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			gst_tag_list_get_string(tags, GST_TAG_VIDEO_CODEC, &str);
			total_str = g_strdup_printf("  codec: %s\n", str ? str : "unknown");
			gtk_text_buffer_insert_at_cursor(text, total_str, -1);
			g_free(total_str);
			g_free(str);
			gst_tag_list_free(tags);
		}
	}

	for (gint i = 0; i < n_audio; ++i)
	{
		tags = NULL;
		/* Retrieve the stream's audio tags */
		g_signal_emit_by_name (data->playbin, "get-audio-tags", i, &tags);
		if (tags)
		{
			total_str = g_strdup_printf ("\naudio stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor (text, total_str, -1);
			g_free (total_str);
			if (gst_tag_list_get_string (tags, GST_TAG_AUDIO_CODEC, &str))
			{
				total_str = g_strdup_printf("  codec: %s\n", str);
				gtk_text_buffer_insert_at_cursor(text, total_str, -1);
				g_free (total_str);
				g_free (str);
			}
			if (gst_tag_list_get_string(tags, GST_TAG_LANGUAGE_CODE, &str))
			{
				total_str = g_strdup_printf ("  language: %s\n", str);
				gtk_text_buffer_insert_at_cursor (text, total_str, -1);
				g_free (total_str);
				g_free (str);
			}
			if (gst_tag_list_get_uint (tags, GST_TAG_BITRATE, &rate))
			{
				total_str = g_strdup_printf ("  bitrate: %d\n", rate);
				gtk_text_buffer_insert_at_cursor (text, total_str, -1);
				g_free (total_str);
			}
			gst_tag_list_free (tags);
		}
	}

	for (gint i = 0; i < n_text; i++)
	{
		tags = NULL;
		/* Retrieve the stream's subtitle tags */
		g_signal_emit_by_name (data->playbin, "get-text-tags", i, &tags);
		if (tags) {
			total_str = g_strdup_printf ("\nsubtitle stream %d:\n", i);
			gtk_text_buffer_insert_at_cursor (text, total_str, -1);
			g_free (total_str);
			if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str)) {
				total_str = g_strdup_printf ("  language: %s\n", str);
				gtk_text_buffer_insert_at_cursor (text, total_str, -1);
				g_free (total_str);
				g_free (str);
			}
			gst_tag_list_free (tags);
		}
	}
}

// called when an "application" message in posted on the bus
static void application_cb(GstBus * bus, GstMessage * msg, custom_data * data)
{
	if (g_strcmp0(gst_structure_get_name(gst_message_get_structure(msg)), "tags-changed") == 0)
		analyze_streams(data);
}

int main(int argc, char * argv[])
{
	gtk_init(&argc, &argv);
	gst_init(&argc, &argv);

	custom_data data;
	memset(&data, 0, sizeof(data));
	data.duration = GST_CLOCK_TIME_NONE;

	data.playbin = gst_element_factory_make("playbin", "playbin");
	if (!data.playbin)
	{
		g_printerr("Not all elements could be created.\n");
		return -1;
	}

	string const ifile = (argc > 1) ? argv[1] : "test.mkv";
	string const uri = "file://" + pwd() + "/" + ifile;
	g_object_set(data.playbin, "uri", uri.c_str(), nullptr);

	g_signal_connect(G_OBJECT(data.playbin), "video-tags-changed", G_CALLBACK(tags_cb), &data);
	g_signal_connect(G_OBJECT(data.playbin), "audio-tags-changed", G_CALLBACK(tags_cb), &data);
	g_signal_connect(G_OBJECT(data.playbin), "text-tags-changed", G_CALLBACK(tags_cb), &data);

	create_ui(&data);

	GstBus * bus = gst_element_get_bus(data.playbin);
	gst_bus_add_signal_watch(bus);
	g_signal_connect(G_OBJECT(bus), "message::error", G_CALLBACK(error_cb), &data);
	g_signal_connect(G_OBJECT(bus), "message::eos", G_CALLBACK(eos_cb), &data);
	g_signal_connect(G_OBJECT(bus), "message::state-changed", G_CALLBACK(state_changed_cb), &data);
	g_signal_connect(G_OBJECT(bus), "message::application", G_CALLBACK(application_cb), &data);
	gst_object_unref(bus);

	// start playing
	GstStateChangeReturn ret = gst_element_set_state(data.playbin, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(data.playbin);
		return -1;
	}

	g_timeout_add_seconds(1, (GSourceFunc)refresh_ui, &data);  // register a function that GLib will call every second

	gtk_main();

	gst_element_set_state(data.playbin, GST_STATE_NULL);
	gst_object_unref(data.playbin);

	return 0;
}
