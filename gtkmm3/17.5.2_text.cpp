// drawing text in pango
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <pangomm.h>

class MyArea : public Gtk::DrawingArea
{
private:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;
	void draw_rectangle(Cairo::RefPtr<Cairo::Context> const & cr, int width, int height);
	void draw_text(Cairo::RefPtr<Cairo::Context> const & cr, int rectangle_width, int rectangle_height);
};

bool MyArea::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	Gtk::Allocation alloc = get_allocation();
	int const width = alloc.get_width();
	int const height = alloc.get_height();

	int const rectangle_width = width;
	int const rectangle_height = height/2;

	// draw black rectangle
	cr->set_source_rgb(0.0, 0.0, 0.0);
	draw_rectangle(cr, rectangle_width, rectangle_height);

	// and some white text
	cr->set_source_rgb(1.0, 1.0, 1.0);
	draw_text(cr, rectangle_width, rectangle_height);

	// flip the image vertically
	Cairo::Matrix T{1.0, 0.0, 0.0, -1.0, 0.0, (double)height};
	cr->transform(T);

	// white rectangle
	cr->set_source_rgb(1.0, 1.0, 1.0);
	draw_rectangle(cr, rectangle_width, rectangle_height);

	// black text
	cr->set_source_rgb(0.0, 0.0, 0.0);
	draw_text(cr, rectangle_width, rectangle_height);

	return true;
}

void MyArea::draw_rectangle(Cairo::RefPtr<Cairo::Context> const & cr, int width, int height)
{
	cr->rectangle(0, 0, width, height);
	cr->fill();
}

void MyArea::draw_text(Cairo::RefPtr<Cairo::Context> const & cr, int rectangle_width, int rectangle_height)
{
	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);

	Glib::RefPtr<Pango::Layout> layout = create_pango_layout("Hi there!");
	layout->set_font_description(font);

	// get the text dimensions
	int text_width, text_height;
	layout->get_pixel_size(text_width, text_height);

	// position the text in the middle
	cr->move_to((rectangle_width - text_width) / 2, (rectangle_height - text_height) / 2);

	layout->show_in_cairo_context(cr);
}


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	Gtk::Window win;
	win.set_title("Drawing text example");

	MyArea area;
	win.add(area);
	area.show();

	return app->run(win);
}
