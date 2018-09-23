// fixed container test
#include <gtkmm.h>
#include <gtkmm/volumebutton.h>

class example_window : public Gtk::Window
{
public:
	example_window()
	{
		set_title("VolumeButton sample");
		set_default_size(400, 300);
		add(_fbox);
		_fbox.put(_volume, 10, 10);
		show_all_children();
	}

private:
	Gtk::Fixed _fbox;
	Gtk::VolumeButton _volume;
};

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "volumebutton.example");
	example_window w;
	return app->run(w);
}
