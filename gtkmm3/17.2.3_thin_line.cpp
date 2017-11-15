#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	void fix_lines(bool fix);

private:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;
	void force_redraw();

	double _fix;
};

class ExampleWindow : public Gtk::Window
{
public:
	ExampleWindow();

private:
	void on_button_toggled();

	MyArea _area_lines;
	Gtk::Grid _container;
	Gtk::CheckButton _button_fixlines;
};

ExampleWindow::ExampleWindow()
	: _button_fixlines{"Fix lines"}
{
	set_title("Thin lines example");

	_container.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	_container.add(_area_lines);
	_container.add(_button_fixlines);
	add(_container);

	_button_fixlines.signal_toggled().connect(
		sigc::mem_fun(*this, &ExampleWindow::on_button_toggled));

	on_button_toggled();

	show_all_children();
}

void ExampleWindow::on_button_toggled()
{
	_area_lines.fix_lines(_button_fixlines.get_active());
}

MyArea::MyArea()
	: _fix{0}
{
	set_size_request(200, 100);
}

void MyArea::fix_lines(bool fix)
{
	_fix = fix ? 0.5 : 0.0;
	force_redraw();
}

bool MyArea::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	Gtk::Allocation allocation = get_allocation();
	int const width = allocation.get_width();
	int const height = allocation.get_height();

	cr->set_line_width(1.0);

	// draw one line, every two pixels
	// without the 'fix', you won't notice any space between the lines,
	// since each one will occupy two pixels (width)
	for (int i = 0; i < width; i += 2)
	{
		cr->move_to(i + _fix, 0);
		cr->line_to(i + _fix, height);
	}

	cr->stroke();

	return true;
}

void MyArea::force_redraw()
{
	Glib::RefPtr<Gdk::Window> win = get_window();
	if (win)
	{
		Gtk::Allocation alloc = get_allocation();
		Gdk::Rectangle r{0, 0, alloc.get_width(), alloc.get_height()};
		win->invalidate_rect(r, false);
	}
}

int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	ExampleWindow window;
	return app->run(window);
}
