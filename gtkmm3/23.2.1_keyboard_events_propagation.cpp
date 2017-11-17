#include <iostream>
#include <gtkmm.h>

using std::cout;

class ExampleWindow : public Gtk::Window
{
public:
	ExampleWindow();

private:
	bool on_key_release_event(GdkEventKey * event) override;

	bool entryKeyRelease(GdkEventKey * event);
	bool gridKeyRelease(GdkEventKey * event);
	bool windowKeyReleaseBefore(GdkEventKey * event);
	bool windowKeyRelease(GdkEventKey * event);

	Gtk::Grid _container;
	Gtk::Label _label;
	Gtk::Entry _entry;
	Gtk::CheckButton _check_button_can_propagate;
};

ExampleWindow::ExampleWindow()
{
	add(_container);
	set_title("Event Propagation");
	set_border_width(10);

	_label.set_label("A label");
	_check_button_can_propagate.set_label("Can Propagate");
	_check_button_can_propagate.set_active();

	// main container
	_container.set_orientation(Gtk::ORIENTATION_VERTICAL);
	_container.add(_label);
	_container.add(_entry);
	_container.add(_check_button_can_propagate);

	// events
	add_events(Gdk::KEY_RELEASE_MASK);
	_entry.signal_key_release_event().connect(sigc::mem_fun(*this, &ExampleWindow::entryKeyRelease));
	_container.signal_key_release_event().connect(sigc::mem_fun(*this, &ExampleWindow::gridKeyRelease));

	// called before the default event signal handler
	signal_key_release_event().connect(sigc::mem_fun(*this, &ExampleWindow::windowKeyReleaseBefore), false);

	// called after the default event signal handler
	signal_key_release_event().connect(sigc::mem_fun(*this, &ExampleWindow::windowKeyRelease));

	show_all_children();
}

bool ExampleWindow::entryKeyRelease(GdkEventKey * event)
{
	cout << "Entry" << std::endl;
	return _check_button_can_propagate.get_active() ? false : true;
}

bool ExampleWindow::gridKeyRelease(GdkEventKey * event)
{
	cout << "Grid" << std::endl;
	return false;  // let it propagate
}

bool ExampleWindow::windowKeyReleaseBefore(GdkEventKey * event)
{
	cout << "Window before" << std::endl;
	return false;
}

bool ExampleWindow::windowKeyRelease(GdkEventKey * event)
{
	cout << "Window after";
	//checking if the entry is on focus, otherwise the label would get changed by pressing keys
	//on the window (when the entry is not on focus), even if m_checkbutton_can_propagate wasn't active
	if (_entry.has_focus())
	{
		_label.set_text(_entry.get_text());
		cout << ", " << _entry.get_text();
	}
	cout << std::endl;

	return true;
}

bool ExampleWindow::on_key_release_event(GdkEventKey * event)
{
	cout << "Window overriden" << std::endl;
	return Gtk::Window::on_key_release_event(event);
}


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	ExampleWindow win;
	return app->run(win);
}
