// prints display resolution in pixels
#include <iostream>
#include <cassert>
#include <X11/Xlib.h>

using std::cout;

int main(int argc, char * argv[])
{
	Display * disp = XOpenDisplay(nullptr);
	assert(disp);
	
	int screen_idx = DefaultScreen(disp);
	unsigned w = DisplayWidth(disp, screen_idx);
	unsigned h = DisplayHeight(disp, screen_idx);
	
	cout << "w=" << w << ", h=" << h << "\n";
	
	XCloseDisplay(disp);
	
	return 0;
}