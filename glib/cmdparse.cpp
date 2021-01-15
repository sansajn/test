// introductory command line parsing sample with glib
#include <glib.h>

static gint repeats = 2,
	max_size = 8;

static gboolean verbose = FALSE,
	beep = FALSE,
	randomize = FALSE;

static gchar * function = "cos";

static GOptionEntry entries[] = {
	{"repeats", 'r', 0, G_OPTION_ARG_INT, &repeats, "Average over N repetitions", "N"},
	{"max-size", 'm', 0, G_OPTION_ARG_INT, &max_size, "Test up to 2^M items", "M"},
	{"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", nullptr},
	{"beep", 'b', 0, G_OPTION_ARG_NONE, &beep, "Beep when done", nullptr},
	{"rand", 0, 0, G_OPTION_ARG_NONE, &randomize, "Randomize the data", nullptr},
	{"funct", 0, 0, G_OPTION_ARG_STRING, &function, "Use specific function", "FUNC"},
	{nullptr}
};

int main(int argc, char * argv[])
{
	GOptionContext * context = g_option_context_new("- test tree model performance");
	g_option_context_add_main_entries(context, entries, "");

	GError * error = nullptr;
	if (!g_option_context_parse(context, &argc, &argv, &error))
	{
		g_print("option parsing failed: %s\n", error->message);
		exit(1);
	}

	g_print("arguments:\n"
		"\trepeats=%d\n"
		"\tmax-size=%d\n"
		"\tverbose=%d\n"
		"\tbeep=%d\n"
		"\trand=%d\n"
		"\tfunct=%s\n",
		repeats, max_size, verbose, beep, randomize, function);

	g_option_context_free(context);

	g_print("done!\n");
	return 0;
}
