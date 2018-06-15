// GdkScreen basic info sample
#include <iostream>
#include <cassert>
#include <gtkmm/window.h>
#include <gdk/gdk.h>

using std::cout;

class window : public Gtk::Window
{
public:
	window();
};

window::window()
{
	set_title("sample window");

	GdkScreen * screen = gdk_screen_get_default();
	assert(screen);

	gint monitor_count = gdk_screen_get_n_monitors(screen);
	cout << "monitors=" << monitor_count << "\n";

	gint width = gdk_screen_get_width(screen);
	gint height = gdk_screen_get_height(screen);
	cout << "width=" << width << ", height=" << height << "\n";
}

int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "gdk_screen.example");
	window w;
	return app->run(w);
}
