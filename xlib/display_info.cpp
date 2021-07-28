/* prints basic display informations like resolution in pixels and more 
see https://tronche.com/gui/x/xlib/display/display-macros.html */
#include <iostream>
#include <cassert>
#include <X11/Xlib.h>

using std::cout;

int main(int argc, char * argv[]) {
	// hostname:number.screen_number
	Display * disp = XOpenDisplay(nullptr);
	assert(disp);
	
	int screen_idx = DefaultScreen(disp);
	int screen_count = XScreenCount(disp);
	
	cout << "default display:\n"
		<< "  number of screens: " << screen_count << "\n";

	for (int i = 0; i < screen_count; ++i) {
		unsigned w = DisplayWidth(disp, screen_idx),
			h = DisplayHeight(disp, screen_idx);

		cout << "    screen #" << i << ":\n"
			<< "      w=" << w << ", h=" << h << "\n";
	}

	XCloseDisplay(disp);
	
	return 0;
}
