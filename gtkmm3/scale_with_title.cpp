// fixed container test
#include <gtkmm.h>

class example_window : public Gtk::Window
{
public:
	example_window()
	{
		set_title("Scale widget sample");
		set_default_size(400, 80);

		_hbox.pack_start(*Gtk::manage(new Gtk::Label{"Width:"}), Gtk::PACK_SHRINK);

		_adj = Gtk::Adjustment::create(3, 1, 10);
		_scale.set_adjustment(_adj);
		_scale.set_digits(1);
		_scale.set_value_pos(Gtk::POS_LEFT);
		_hbox.pack_start(_scale);

		add(_hbox);
		show_all_children();
	}

private:
	Gtk::HBox _hbox;
	Glib::RefPtr<Gtk::Adjustment> _adj;
	Gtk::HScale _scale;
};

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "fixed.example");
	example_window w;
	return app->run(w);
}
