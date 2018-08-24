// ukazka XMoveResizeWindow()
#include <iostream>
#include <cstring>
#include <cassert>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

using std::cout;


int main(int argc, char * argv[])
{
	char * appname = argv[0];
	char * window_name = "Hello, X Window System!";
	char * icon_name = "HelloX";

	Display * disp = XOpenDisplay(nullptr);
	assert(disp);

	/*  Get screen size from display structure macro  */

	int screen_num = DefaultScreen(disp);
	unsigned display_width = DisplayWidth(disp, screen_num);
	unsigned display_height = DisplayHeight(disp, screen_num);

	// Set initial window size and position, and create it
	int x = 0, y = 0;
	unsigned width = 800, height = 600;
	unsigned int border_width = 0;

	// bez XSizeHints nebude fungovat posunutie okna na x,y
	Window win = XCreateSimpleWindow(disp, RootWindow(disp, screen_num),
		x, y, width, height, border_width,
		BlackPixel(disp, screen_num), WhitePixel(disp, screen_num));

	// set hints for window manager before mapping window
	XSizeHints * size_hints = XAllocSizeHints();
	size_hints->min_width = 200;
	size_hints->min_height = 100;
	size_hints->flags = PPosition|PSize|PMinSize;

	XWMHints * wm_hints = XAllocWMHints();
	wm_hints->flags = StateHint|InputHint;
	wm_hints->initial_state = NormalState;
	wm_hints->input = True;

	XClassHint * class_hints = XAllocClassHint();
	class_hints->res_name = appname;
	class_hints->res_class = "hellox";

	XTextProperty window_name_prop;
	Status res = XStringListToTextProperty(&window_name, 1, &window_name_prop);
	assert(res);

	XTextProperty icon_name_prop;
	res = XStringListToTextProperty(&icon_name, 1, &icon_name_prop);
	assert(res);

	XSetWMProperties(disp, win, &window_name_prop, &icon_name_prop, argv, argc,
		size_hints, wm_hints, class_hints);

	// Choose which events we want to handle
	XSelectInput(disp, win, ExposureMask|KeyPressMask|ButtonPressMask|StructureNotifyMask);

	XFontStruct * font_info = XLoadQueryFont(disp, "9x15");  // load a font called "9x15"
	assert(font_info);

	XGCValues values;
	GC gc = XCreateGC(disp, win, 0, &values);  // create graphics context

	XSetFont(disp, gc, font_info->fid);
	XSetForeground(disp, gc, BlackPixel(disp, screen_num));

	XMapWindow(disp, win);  // display window

	XMoveResizeWindow(disp, win, 0, 0, 5360, 1440);

	bool running = true;
	while (running)   // event loop
	{
		XEvent event;
		XNextEvent(disp, &event);

		switch (event.type)
		{
			case Expose:
			{
				char const * msg = "Hello, X Window System!";

				if (event.xexpose.count != 0)
					break;

				int text_w = XTextWidth(font_info, msg, strlen(msg));
				int x = (width - text_w)/2;

				int font_h = font_info->ascent + font_info->descent;
				int y = (height + font_h)/2;

				XDrawString(disp, win, gc, x, y, msg, strlen(msg));

				break;
			}

			case ConfigureNotify:
			{
				width  = event.xconfigure.width;
				height = event.xconfigure.height;
				break;
			}

			case KeyPress:
			{
				char ch;
				KeySym keysym;
				XComposeStatus xcompstat;
				XLookupString(&event.xkey, &ch, 1, &keysym, &xcompstat);
				if (ch == 'q')
					running = false;
				break;
			}
		}  // switch
	}

	XUnloadFont(disp, font_info->fid);
	XFreeGC(disp, gc);
	XCloseDisplay(disp);

	return EXIT_SUCCESS;   /*  We shouldn't get here  */
}
