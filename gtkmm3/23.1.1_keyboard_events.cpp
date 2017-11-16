#include <gtkmm.h>

class ExampleWindow : public Gtk::Window
{
public:
	ExampleWindow();

private:
	bool on_key_press_event(GdkEventKey * event) override;

	Gtk::Grid _container;
	Gtk::RadioButton _first;
	Gtk::RadioButton _second;
};

ExampleWindow::ExampleWindow()
{
	set_title("Keyboard Events");
	set_border_width(10);
	add(_container);

	// radio buttons
	_first.set_label("First");
	_second.set_label("Second");

	_second.join_group(_first);
	_first.set_active();

	// main container
	_container.add(_first);
	_container.add(_second);

	// events
	add_events(Gdk::KEY_PRESS_MASK);

	show_all_children();
}

bool ExampleWindow::on_key_press_event(GdkEventKey * event)
{
	// GDK_MOD1_MASK -> the 'alt' key
	// GDK_KEY_1, GDK_KEY_2 -> '1' and '2' keys

	// select the first radio button, when we press alt+1
	if ((event->keyval == GDK_KEY_1)
		&& (event->state & (GDK_SHIFT_MASK|GDK_CONTROL_MASK|GDK_MOD1_MASK)) == GDK_MOD1_MASK)
	{
		_first.set_active();
		return true;  // cancels the propagation of the event
	}
	else if ((event->keyval == GDK_KEY_2)
		&& (event->state & (GDK_SHIFT_MASK|GDK_CONTROL_MASK|GDK_MOD1_MASK)) == GDK_MOD1_MASK)
	{
		_second.set_active();
		return true;  // cancels the propagation of the event
	}
	else if (event->keyval == GDK_KEY_Escape)
	{
		hide();  // close the window, when the 'esc' key is pressed
		return true;
	}

	return Gtk::Window::on_key_press_event(event);
}


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	ExampleWindow win;
	return app->run(win);
}
