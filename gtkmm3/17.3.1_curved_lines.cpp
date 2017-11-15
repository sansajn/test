#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();

private:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;
};

MyArea::MyArea()
{
	set_size_request(200, 100);
}

bool MyArea::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	Gtk::Allocation allocation = get_allocation();
	int const width = allocation.get_width();
	int const height = allocation.get_height();

	double
		x0 = 0.1, y0 = 0.5,  // start point
		x1 = 0.4, y1 = 0.9,  // control point #1
		x2 = 0.6, y2 = 0.1,  // control point #2
		x3 = 0.9, y3 = 0.5;  // end point

	cr->scale(width, height);  // scale to unit square

	cr->set_line_width(0.05);

	// draw curve
	cr->move_to(x0, y0);
	cr->curve_to(x1, y1, x2, y2, x3, y3);
	cr->stroke();

	// show control points
	cr->set_source_rgba(1, 0.2, 0.2, 0.6);
	cr->move_to(x0, y0);
	cr->line_to(x1, y1);
	cr->move_to(x2, y2);
	cr->line_to(x3, y3);
	cr->stroke();

	return true;
}

int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	Gtk::Window win;
	win.set_title("DrawingArea");

	MyArea area;
	win.add(area);
	area.show();

	return app->run(win);
}
