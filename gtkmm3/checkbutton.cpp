// checkbutton sample

#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/checkbutton.h>

using std::cout;

class example_window : public Gtk::Window
{
public:
	example_window();
	
private:
	void on_button_clicked();
	
	Gtk::CheckButton _btn;
};

example_window::example_window()
	: _btn{"something"}
{
	set_title("checkbutton example");
	set_border_width(10);
	
	_btn.signal_clicked().connect(sigc::mem_fun(*this, &example_window::on_button_clicked));
	
	add(_btn);
	
	show_all_children();
}
	
void example_window::on_button_clicked()
{
	cout << "the button was clicked: state=" << (_btn.get_active() ? "true" : "false") << std::endl;
}


int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	example_window w;
	return app->run(w);
}
