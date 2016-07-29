// vypise verziu gstreamer kniznice
#include <iostream>
#include <cstdio>
#include <gst/gst.h>

using std::cout;

int main(int argc, char * argv[])
{
	gst_init(&argc, &argv);
	
	guint major, minor, micro, nano;
	gst_version(&major, &minor, &micro, &nano);
	
	gchar const * nano_str = nullptr;
	if (nano == 1)
		nano_str = "(CVS)";
	else if (nano == 2)
		nano_str = "(Prerelease)";
	else
		nano_str = "";
	
	cout << "This program is linked against GStreamer " << major << "." << minor 
		<< "." << micro << " " << nano_str << std::endl;
	
	return 0;
}
