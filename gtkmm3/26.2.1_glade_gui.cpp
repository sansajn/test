// shows how to load a Glade file at runtime and access the widgets with Gtk::Builder
#include <gtkmm.h>
#include <iostream>

using std::cerr;

Gtk::Dialog * dialog = nullptr;

static void on_button_clicked()
{
	if (dialog)
		dialog->hide();  // hide() will cause main::run() to end
}

int main(int argc, char * argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	auto builder = Gtk::Builder::create();
	
	try {
		builder->add_from_file("basic.glide");
	}
	catch (Glib::FileError const & e) {
		cerr << "FileError: " << e.what() << std::endl;
		return 1;
	}
	catch (Glib::MarkupError const & e) {
		cerr << "MarkupError: " << e.what() << std::endl;
		return 1;
	}
	catch (Gtk::BuilderError const & e) {
		cerr << "BuilderError: " << e.what() << std::endl;
		return 1;
	}
	
	// get dialog
	builder->get_widget("dialog", dialog);
	if (dialog)
	{
		Gtk::Button * btn = nullptr;
		builder->get_widget("quit_button", btn);
		if (btn)
			btn->signal_clicked().connect(sigc::ptr_fun(on_button_clicked));
		
		app->run(*dialog);
	}
	
	delete dialog;
	
	return 0;
}
