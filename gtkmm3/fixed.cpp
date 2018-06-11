// fixed container test
#include <gtkmm.h>

class example_window : public Gtk::Window
{
public:
	example_window()
		: _btn1{"hello!"}
		, _btn2{"Jane"}
	{
		set_title("Fixed container sample");
		set_default_size(400, 300);
		add(_fbox);
		_fbox.put(_btn1, 10, 10);
		_fbox.put(_btn2, 80, 80);
		show_all_children();
	}

private:
	Gtk::Fixed _fbox;
	Gtk::Button _btn1, _btn2;
};

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "fixed.example");
	example_window w;
	return app->run(w);
}
