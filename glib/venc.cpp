// introductory command line parsing sample with glib
#include <glib.h>

static gboolean verbose = FALSE;

static gchar const * encoder = "vp9",
 	* log = "",
	* output = "";

static GOptionEntry entries[] = {
	{"encoder", 'e', 0, G_OPTION_ARG_STRING, &encoder, "used video encoder, available encoders are h264, h265, vp8 or vp9", nullptr},
	{"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose output", nullptr},
	{"log", 0, 0, G_OPTION_ARG_STRING, &log, "create log file", nullptr},
	{"output", 'o', 0, G_OPTION_ARG_STRING, &output, "save converted video as arg", nullptr},
	{nullptr}
};

int main(int argc, char * argv[])
{
	GOptionContext * context = g_option_context_new("");
	g_option_context_add_main_entries(context, entries, "");

	GError * error = nullptr;
	if (!g_option_context_parse(context, &argc, &argv, &error))
	{
		g_print("option parsing failed: %s\n", error->message);
		exit(1);
	}

	g_print("arguments:\n"
		"\tencoder=%s\n"
		"\tverbose=%d\n"
		"\tlog=%s\n"
		"\toutput=%s\n",
		encoder, verbose, log, output);

	g_option_context_free(context);

	g_print("done!\n");
	return 0;
}
