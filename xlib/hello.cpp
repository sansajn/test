// vytvori okno v xlib, 1-1 s knihy Linux 3D Graphics Programming
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

int main(int argc, char * argv[])
{
	Display * dpy = XOpenDisplay(nullptr);
	Visual * vis = DefaultVisual(dpy, 0);

	Window w = XCreateWindow(
		dpy,
		DefaultRootWindow(dpy),
		100, 100,
		800, 600,
		0,
		CopyFromParent,
		CopyFromParent,
		vis,
		0, nullptr);

	XStoreName(dpy, w, "hello xlib");
	XMapWindow(dpy, w);
	XSelectInput(dpy, w, KeyPressMask);

	while (1)
	{
		char ch;
		XEvent event;
		if (XCheckWindowEvent(dpy, w, KeyPressMask, &event))
		{
			KeySym keysym;
			XComposeStatus xcompstat;
			XLookupString(&event.xkey, &ch, 1, &keysym, &xcompstat);
			switch (ch)
			{
				case 'q':
					exit(0);
			}
		}
	}

	return 0;
}
