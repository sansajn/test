/* Gtk::ListViewText sample
https://github.com/GNOME/gtkmm-documentation/tree/master/examples/book/treeview/listviewtext */
#include <iostream>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/listviewtext.h>

class example_window : public Gtk::Window
{
public:
	example_window();
	
protected:
	void on_button_quit();
	
	Gtk::Box _vbox;
	Gtk::ScrolledWindow _scrolled_window;
	Gtk::ListViewText _list_view_text;
	Gtk::ButtonBox _button_box;
	Gtk::Button _button_quit;
};

example_window::example_window()
	: _vbox{Gtk::Orientation::ORIENTATION_VERTICAL}
	, _list_view_text{3}
	, _button_quit{"Quit"}
{
	set_title("Gtk::ListViewText example");
	set_default_size(400, 200);
	
//	_vbox.set_margin(6);
	add(_vbox);
	
	// add the TreeView, inside a ScrolledWindow, with the button underneath
	_scrolled_window.add(_list_view_text);
	
	// only show the scrollbars when they are necessary
	_scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC, Gtk::PolicyType::POLICY_AUTOMATIC);
	
	_vbox.pack_start(_scrolled_window, Gtk::PackOptions::PACK_EXPAND_WIDGET);
	_vbox.pack_start(_button_box, Gtk::PackOptions::PACK_SHRINK);
	
	_button_box.pack_start(_button_quit, Gtk::PackOptions::PACK_SHRINK);
//	_button_box.set_margin(5);
	_button_box.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_END);
	_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &example_window::on_button_quit));
	
	// fill the ListViewText
	_list_view_text.set_column_title(0, "City");
	_list_view_text.set_column_title(1, "Temperature (celsius)");
	_list_view_text.set_column_title(2, "Forecast");
	
	guint row_number = _list_view_text.append();
	_list_view_text.set_text(row_number, 0, "Madrid");
	_list_view_text.set_text(row_number, 1, "30");
	_list_view_text.set_text(row_number, 2, "cloudy");
	
	row_number = _list_view_text.append();
	_list_view_text.set_text(row_number, 0, "Lisbon");
	_list_view_text.set_text(row_number, 1, "25");
	_list_view_text.set_text(row_number, 2, "rainy");

	show_all();
}

void example_window::on_button_quit()
{
	hide();
}

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create("org.gtkmm.example");
	example_window w;
	return app->run(w, argc, argv);
}
