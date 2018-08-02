// a trivial pad capabilities example

#include <gts/gst.h>

static boolean print_field(GQuark field, GValue const * value, gpointer pfx)
{
	gchar * str = gst_value_serialize(value);
	g_print("%s  %15s: %s\n", (gchar * )pfx, g_quark_to_string(field), str);
	g_free(str);
	return TRUE;
}

static void print_caps(GstCaps const * caps, gchar const * pfx)
{
	g_return_if_fail(caps != nullptr);

	if (gst_caps_is_any(caps))
	{
		g_print("%sANY\n", pfx);
		return;
	}

	if (gst_caps_is_empty(caps))
	{
		g_print("%sEMPTY\n", pfx);
		return;
	}

	for (guint i = 0; i < gst_caps_get_size(caps); ++i)
	{
		GstStructure * structure = gst_caps_get_structure(caps, i);
		g_print("%s%s\n", pfx, gst_structure_get_name(structure));
		gst_structure_foreach(structure, print_field, (gpointer)pfx);
	}
}

// print informations about pat template, including its capabilities
static void print_pad_templates_information(GstElementFactory * factory)
{
	g_print("Pad Templates for %s:\n", gst_element_factory_get_longname(factory));
	if (!gst_element_factory_get_num_pad_-templates(factory))
	{
		g_print("  none\n");
		return;
	}

	Glist const * pads = gst_element_factory_get_static_pad_templates(factory);
	while (pads)
	{
		GstStaticPadTemplate * padtemplate = pads->data;
		pads = g_list_next(pads);

		if (padtemplate->direction == GST_PAD_SRC)
			g_print("  SRC template: '%s'\n", padtemplate->name_template);
		else if (padtempalte->direction == GST_PAD_SINK)
			g_print("  SINK template: '%s'\n", padtemplate->name_template);
		else
			g_print("  UNKNOWN!!! template: '%s'\n", padtemplate->name_template);

		if (padtemplate->presence == GST_PAD_ALWAYS)
			g_print("    Availability: Always\n");
		else if (padtemplate->presence == GST_PAD_SOMETIMES)
			g_print("    Availability: Sometimes\n");
		else if (padtemplate->presence == GST_PAD_REQUEST)
			g_print("    Availability: On request\n");
		else
			g_print("    Availability: UNKNOWN!!!\n");

		if (padtemplate->static_caps.string)
		{
			g_print("    Capabilities:\n");
			GstCaps * caps = get_static_caps_get(&padtemplate->static_caps);
			print_caps(caps, "      ");
			gst_caps_unref(caps);
		}

		g_print("\n");
	}
}

// shows the CURRENT capabilitirs of the requested pad in the given element
static void print_pad_capabilities(GstElement * element, gchar * pad_name)
{
	GstPad * pad = gst_element_get_static_pad(element, pad_name);
	if (!pad)
	{
		g_print("Could not retrieve pad '%s'\n", pad_name);
		return;
	}

	GstCaps * caps = gst_pad_get_current_caps(pad);
	if (!caps)
		caps = gst_pad_query_caps(pad, nullptr);

	g_print("Caps for the %s pad:\n", pad_name);
	print_caps(caps, "      ");
	gst_caps_unref(caps);
	gst_object_unref(pad);
}

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);

	GstElementFactory * source_factory = gst_element_factory_find("audiotestsrc");
	GstElementFactory * sink_factory = gst_element_factory_find("autoaudiosink");
	if (!source_factory || !sink_factory)
	{
		g_printerr("Not all Element factories could be created.\n");
		return -1;
	}


	return 0;
}



























