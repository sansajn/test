#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

class MyArea : public Gtk::DrawingArea
{
protected:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;
};


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
		"org.gtkmm.example");

	Gtk::Window win;
	win.set_title("DrawingArea");

	MyArea area;
	win.add(area);
	area.show();

	return app->run(win);
}

bool MyArea::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	Gtk::Allocation allocation = get_allocation();
	int const width = allocation.get_width();
	int const height = allocation.get_height();

	int xc = width/2;
	int yc = height/2;

	cr->set_line_width(10.0);

	cr->set_source_rgb(0.8, 0.0, 0.0);
	cr->move_to(0, 0);
	cr->line_to(xc, yc);
	cr->line_to(0, height);
	cr->move_to(xc, yc);
	cr->line_to(width, yc);
	cr->stroke();

	return true;
}
