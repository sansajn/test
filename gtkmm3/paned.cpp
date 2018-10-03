// Gtk::Paned sample
#include <gtkmm.h>
#include <gtkmm/paned.h>

class example_window : public Gtk::Window
{
public:
	example_window()
		: _btn1{"hello!"}
		, _btn2{"Jane"}
	{
		set_title("Paned sample");
		set_default_size(400, 300);
		add(_paned);
		_paned.add1(_btn1);
		_paned.add2(_btn2);
		show_all_children();
	}

private:
	Gtk::VPaned _paned;
	Gtk::Button _btn1, _btn2;
};

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "fixed.example");
	example_window w;
	return app->run(w);
}


