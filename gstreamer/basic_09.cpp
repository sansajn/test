// media information gathering
#include <string>
#include <cstring>
#include <gst/gst.h>
#include <gst/pbutils/pbutils.h>
#include <unistd.h>

using std::string;

struct custom_data
{
	GstDiscoverer * discoverer;
	GMainLoop * loop;
};


void print_tag_foreach(GstTagList const * tags, gchar const * tag, gpointer user_data);
void print_stream_info(GstDiscovererStreamInfo * info, gint depth);
void print_topology(GstDiscovererStreamInfo * info, gint depth);
void on_discovered_cb(GstDiscoverer * discoverer, GstDiscovererInfo * info, GError * err, custom_data * data);
void on_finished_cb(GstDiscoverer * discoverer, custom_data * data);
string pwd();


int main(int argc, char * argv[])
{
	string uri = "file://" + pwd() + "/" + (argc > 1 ? argv[1] : "lara.mp4");

	custom_data data;
	memset(&data, 0, sizeof(data));

	gst_init(&argc, &argv);

	g_print("Discovering '%s'\n", uri.c_str());

	GError * err = nullptr;
	data.discoverer = gst_discoverer_new(5 * GST_SECOND, &err);
	if (!data.discoverer)
	{
		g_print("Error creating discoverer instance: %s\n", err->message);
		g_clear_error(&err);
		return -1;
	}

	g_signal_connect(data.discoverer, "discovered", G_CALLBACK(on_discovered_cb), &data);
	g_signal_connect(data.discoverer, "finished", G_CALLBACK(on_finished_cb), &data);

	gst_discoverer_start(data.discoverer);

	if (!gst_discoverer_discover_uri_async(data.discoverer, uri.c_str()))
	{
		g_print("Failed to start discovering URI '%s'\n", uri.c_str());
		g_object_unref(data.discoverer);
		return -1;
	}

	data.loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(data.loop);

	gst_discoverer_stop(data.discoverer);

	g_object_unref(data.discoverer);
	g_main_loop_unref(data.loop);

	return 0;
}

void print_tag_foreach(GstTagList const * tags, gchar const * tag, gpointer user_data)
{
	GValue val = {0, };
	gint depth = GPOINTER_TO_INT(user_data);

	gst_tag_list_copy_value(&val, tags, tag);

	gchar * str;
	if (G_VALUE_HOLDS_STRING(&val))
		str = g_value_dup_string(&val);
	else
		str = gst_value_serialize(&val);

	g_print("%*s%s: %s\n", 2 * depth, " ", gst_tag_get_nick(tag), str);
	g_free(str);

	g_value_unset(&val);
}

void print_stream_info(GstDiscovererStreamInfo * info, gint depth)
{
	gchar * desc;

	GstCaps * caps = gst_discoverer_stream_info_get_caps(info);
	if (caps)
	{
		if (gst_caps_is_fixed(caps))
			desc = gst_pb_utils_get_codec_description(caps);
		else
			desc = gst_caps_to_string(caps);

		gst_caps_unref(caps);
	}

	g_print("%*s%s: %s\n", 2*depth, " ",
		gst_discoverer_stream_info_get_stream_type_nick(info),
		(desc ? desc : ""));

	if (desc)
	{
		g_free(desc);
		desc = nullptr;
	}

	GstTagList const * tags = gst_discoverer_stream_info_get_tags(info);
	if (tags)
	{
		g_print("%*sTags:\n", 2*(depth+1), " ");
		gst_tag_list_foreach(tags, print_tag_foreach, GINT_TO_POINTER(depth + 2));
	}
}

void print_topology(GstDiscovererStreamInfo * info, gint depth)
{
	if (!info)
		return;

	print_stream_info(info, depth);

	GstDiscovererStreamInfo * next = gst_discoverer_stream_info_get_next(info);
	if (next)
	{
		print_topology(next, depth + 1);
		gst_discoverer_stream_info_unref(next);
	}
	else if (GST_IS_DISCOVERER_CONTAINER_INFO(info))
	{
		GList * streams = gst_discoverer_container_info_get_streams(GST_DISCOVERER_CONTAINER_INFO(info));
		for (GList * tmp = streams; tmp; tmp = tmp->next)
		{
			GstDiscovererStreamInfo * tmpinf = (GstDiscovererStreamInfo *)tmp->data;
			print_topology(tmpinf, depth + 1);
		}
		gst_discoverer_stream_info_list_free(streams);
	}
}

void on_discovered_cb(GstDiscoverer * discoverer, GstDiscovererInfo * info, GError * err, custom_data * data)
{
	gchar const * uri = gst_discoverer_info_get_uri(info);

	GstDiscovererResult result = gst_discoverer_info_get_result(info);
	switch (result)
	{
		case GST_DISCOVERER_URI_INVALID:
			g_print("Invalid URI '%s'\n", uri);
			break;

		case GST_DISCOVERER_ERROR:
			g_print("Discoverer error: %s\n", err->message);
			break;

		case GST_DISCOVERER_TIMEOUT:
			g_print("Timeout\n");
			break;

		case GST_DISCOVERER_BUSY:
			g_print("Busy\n");
			break;

		case GST_DISCOVERER_MISSING_PLUGINS: {
			GstStructure const * s = gst_discoverer_info_get_misc(info);
			gchar * str = gst_structure_to_string(s);
			g_print("Missing plugins: %s\n", str);
			g_free(str);
			break;
		}

		case GST_DISCOVERER_OK:
			g_print("Discovered '%s'\n", uri);
			break;
	}

	if (result != GST_DISCOVERER_OK)
	{
		g_print("Thsi URI cannot be played\n");
		return;
	}

	g_print("\nDuration: %" GST_TIME_FORMAT "\n", GST_TIME_ARGS(gst_discoverer_info_get_duration(info)));

	GstTagList const * tags = gst_discoverer_info_get_tags(info);
	if (tags)
	{
		g_print("Tags:\n");
		gst_tag_list_foreach(tags, print_tag_foreach, GINT_TO_POINTER(1));
	}

	g_print("Seekable: %s\n", (gst_discoverer_info_get_seekable(info) ? "yes" : "no"));
	g_print("\n");

	GstDiscovererStreamInfo * sinfo = gst_discoverer_info_get_stream_info(info);
	if (!sinfo)
		return;

	g_print("Stream information:\n");
	print_topology(sinfo, 1);

	gst_discoverer_stream_info_unref(sinfo);

	g_print("\n");
}

void on_finished_cb(GstDiscoverer * discoverer, custom_data * data)
{
	g_print("Finished discovering\n");
	g_main_loop_quit(data->loop);
}

string pwd()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	return cwd;
}
