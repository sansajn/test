// mouse events example
#include <vector>
#include <iostream>
#include <boost/format.hpp>
#include <gtkmm.h>
#include <gdkmm.h>
#include <cairomm/cairomm.h>
#include <sigc++/sigc++.h>
#include <gdk/gdkkeysyms.h>


using std::vector;
using std::cout;

struct line
{
	double x1, y1, x2, y2;
};

class example_window : public Gtk::Window
{
public:
	example_window();

private:
	bool canvas_button_press_event(GdkEventButton * event);
	bool canvas_button_release_event(GdkEventButton * event);
	bool canvas_motion_notify_event(GdkEventMotion * event);
	bool canvas_draw(Cairo::RefPtr<Cairo::Context> const & cr);
	bool on_key_press_event(GdkEventKey * event) override;
	void find_nearest_line_point();
	void force_redraw();

	vector<line> _lines;
	bool _new_line;
	double _x, _y;
	double _cur_x, _cur_y;
	bool _near_line_point;
	double _npoint_x, _npoint_y;
	Gtk::DrawingArea _canvas;
};

example_window::example_window()
	: _new_line{true}
	, _x{0.0}, _y{0.0}
	, _cur_x{0.0}, _cur_y{0.0}
	, _near_line_point{false}
	, _npoint_x{0.0}, _npoint_y{0.0}
{
	set_title("Paint");

	_canvas.signal_draw().connect(sigc::mem_fun(*this, &example_window::canvas_draw));

	// len BUTTON_RELEASE_MASK nejede, musim pocuvat aj BUTTON_PRESS_MASK
	_canvas.add_events(Gdk::POINTER_MOTION_MASK|Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK);
	_canvas.signal_motion_notify_event().connect(sigc::mem_fun(*this, &example_window::canvas_motion_notify_event));
	_canvas.signal_button_release_event().connect(sigc::mem_fun(*this, &example_window::canvas_button_release_event));
	_canvas.signal_button_press_event().connect(sigc::mem_fun(*this, &example_window::canvas_button_press_event));

	add(_canvas);
	show_all_children();
}

bool example_window::canvas_button_press_event(GdkEventButton * event)
{
	return false;  // allow propagation
}


bool example_window::canvas_button_release_event(GdkEventButton * event)
{
	if (event->button != 1 || event->type != Gdk::BUTTON_RELEASE)
		return false;  // allow propagate

	if (_new_line)
	{
		if (_near_line_point)
		{
			_x = _npoint_x;
			_y = _npoint_y;
		}
		else
		{
			_x = event->x;
			_y = event->y;
		}

		_new_line = false;
	}
	else
	{
		double x, y;
		if (_near_line_point)
		{
			x = _npoint_x;
			y = _npoint_y;
		}
		else
		{
			x = event->x;
			y = event->y;
		}

		_lines.push_back(line{_x, _y, x, y});
		_x = x;
		_y = y;
	}

	force_redraw();

	return true;
}

bool example_window::canvas_motion_notify_event(GdkEventMotion * event)
{
	set_title(boost::str(boost::format{
		"example window (%1%,%2%)"} % event->x % event->y));

	_cur_x = event->x;
	_cur_y = event->y;

	find_nearest_line_point();

	force_redraw();

	return true;  // do not propagate
}

bool example_window::on_key_press_event(GdkEventKey * event)
{
	if (event->keyval == GDK_KEY_Escape)
	{
		_new_line = true;
		force_redraw();
	}
}

bool example_window::canvas_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	for (line const & l : _lines)   // draw lines
	{
		cr->move_to(l.x1, l.y1);
		cr->line_to(l.x2, l.y2);
		cr->stroke();
	}

	if (!_new_line)  // draw scatch
	{
		cr->move_to(_x, _y);
		cr->line_to(_cur_x, _cur_y);
		cr->stroke();
	}

	if (_near_line_point)  // end point join
	{
		// constexptr ?
		double const HIGHLIGHT_WIDTH = 8.0;
		cr->rectangle(_npoint_x - HIGHLIGHT_WIDTH/2.0, _npoint_y - HIGHLIGHT_WIDTH/2.0, HIGHLIGHT_WIDTH, HIGHLIGHT_WIDTH);
		cr->stroke();
	}

	return true;
}

bool near(double x1, double y1, double x2, double y2)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return (dx*dx + dy*dy) <= 16.0;
}

void example_window::find_nearest_line_point()
{
	_near_line_point = false;

	// ignore newly created points
	if (!_new_line && near(_x, _y, _cur_x, _cur_y))
		return;

	for (line const & l : _lines)
	{
		if (near(l.x1, l.y1, _cur_x, _cur_y))
		{
			_near_line_point = true;
			_npoint_x = l.x1;
			_npoint_y = l.y1;
			return;  // onnly one end point can be highlighted
		}

		if (near(l.x2, l.y2, _cur_x, _cur_y))
		{
			_near_line_point = true;
			_npoint_x = l.x2;
			_npoint_y = l.y2;
			return;
		}
	}
}

void example_window::force_redraw()
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
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "gtkmm3.mouse_events.example");
	example_window w;
	w.set_default_size(800, 600);
	return app->run(w);
}
