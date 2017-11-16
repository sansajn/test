// clock sample from 'The Drawing Area Widget' chapter
#include <ctime>
#include <cmath>
#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <cairomm/context.h>
#include <glibmm/main.h>

class Clock : public Gtk::DrawingArea
{
public:
	Clock();

private:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;
	bool on_timeout();

	double _radius;
	double _line_width;
};

Clock::Clock()
	: _radius{0.42}, _line_width{0.05}
{
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Clock::on_timeout), 1000);
}

bool Clock::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	Gtk::Allocation alloc = get_allocation();
	int const width = alloc.get_width();
	int const height = alloc.get_height();

	// scale to unit square and translate (0,0) to be (0.5, 0.5)
	cr->scale(width, height);
	cr->translate(0.5, 0.5);
	cr->set_line_width(_line_width);

	cr->save();
	cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);  // green
	cr->paint();
	cr->restore();
	cr->arc(0, 0, _radius, 0, 2*M_PI);
	cr->save();
	cr->set_source_rgba(1.0, 1.0, 1.0, 0.8);
	cr->fill_preserve();
	cr->restore();
	cr->stroke_preserve();
	cr->clip();

	// clock ticks
	for (int i = 0; i < 12; ++i)
	{
		double inset = 0.05;

		cr->save();
		cr->set_line_cap(Cairo::LINE_CAP_ROUND);

		if (i % 3 != 0)
		{
			inset *= 0.8;
			cr->set_line_width(0.03);
		}

		cr->move_to(
			(_radius - inset) * cos(i*M_PI/6),
			(_radius - inset) * sin(i*M_PI/6));

		cr->line_to(
			_radius * cos(i*M_PI/6),
			_radius * sin(i*M_PI/6));

		cr->stroke();
		cr->restore();
	}

	// store the current time
	time_t rawtime;
	time(&rawtime);
	struct tm * time_info = localtime(&rawtime);

	// compute the angles of the indicators of our clock
	double minutes = time_info->tm_min * M_PI/30;
	double hours = time_info->tm_hour * M_PI/6;
	double seconds = time_info->tm_sec * M_PI/30;

	cr->save();
	cr->set_line_cap(Cairo::LINE_CAP_ROUND);

	// draw the seconds hand
	cr->save();
	cr->set_line_width(_line_width/3);
	cr->set_source_rgba(0.7, 0.7, 0.7, 0.8);  // gray
	cr->move_to(0, 0);
	cr->line_to(sin(seconds) * (_radius*0.9), -cos(seconds) * (_radius*0.9));
	cr->stroke();
	cr->restore();

	// draw the minutes hand
	cr->set_source_rgba(0.117, 0.337, 0.612, 0.9);  // blue
	cr->move_to(0, 0);
	cr->line_to(sin(minutes + seconds/60) * (_radius*0.8),
		-cos(minutes + seconds/60) * (_radius*0.8));
	cr->stroke();

	// draw the hours hand
	cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);  // green
	cr->move_to(0, 0);
	cr->line_to(sin(hours + minutes/12.0) * (_radius*0.5),
		-cos(hours + minutes/12.0) * (_radius*0.5));
	cr->stroke();
	cr->restore();

	// draw a little dot in the middle
	cr->arc(0, 0, _line_width / 3.0, 0, 2*M_PI);
	cr->fill();

	return true;
}

bool Clock::on_timeout()
{
	// force redraw the entire clock
	auto win = get_window();
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

	Gtk::Window win;
	win.set_title("Cairomm Clock");

	Clock c;
	win.add(c);
	c.show();

	return app->run(win);
}



