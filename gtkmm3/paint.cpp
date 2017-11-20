// mouse events example
#include <vector>
#include <iostream>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/format.hpp>
#include <gtkmm.h>
#include <gdkmm.h>
#include <cairomm/cairomm.h>
#include <sigc++/sigc++.h>
#include <gdk/gdkkeysyms.h>

namespace bg = boost::geometry;

using std::vector;
using std::cout;

using bg::get;
using bg::distance;
using point_xy = bg::model::d2::point_xy<double>;


struct line
{
	point_xy p1, p2;
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
	point_xy _line_begin;
	point_xy _curpos;
	bool _near_line_point;
	point_xy _nearpos;
	Gtk::DrawingArea _canvas;
};

example_window::example_window()
	: _new_line{true}
	, _line_begin{0.0, 0.0}
	, _curpos{0.0, 0.0}
	, _near_line_point{false}
	, _nearpos{0.0, 0.0}
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
	if (event->button != 1)
		return false;  // allow propagate

	if (_new_line)
	{
		_line_begin = _near_line_point ? _nearpos : point_xy{event->x, event->y};
		_new_line = false;
	}
	else
	{
		point_xy line_end = _near_line_point ? _nearpos : point_xy{event->x, event->y};
		_lines.push_back(line{_line_begin, line_end});
		_line_begin = line_end;
	}

	force_redraw();

	return true;
}

bool example_window::canvas_motion_notify_event(GdkEventMotion * event)
{
	set_title(boost::str(boost::format{
		"example window (%1%,%2%)"} % event->x % event->y));

	_curpos = point_xy{event->x, event->y};

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
		return true;
	}

	return false;  // allow propagate
}

bool example_window::canvas_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	for (line const & l : _lines)   // draw lines
	{
		cr->move_to(get<0>(l.p1), get<1>(l.p1));
		cr->line_to(get<0>(l.p2), get<1>(l.p2));
		cr->stroke();
	}

	if (!_new_line)  // draw scatch
	{
		cr->move_to(get<0>(_line_begin), get<1>(_line_begin));
		cr->line_to(get<0>(_curpos), get<1>(_curpos));
		cr->stroke();
	}

	if (_near_line_point)  // end point join
	{
		// constexptr ?
		double const HIGHLIGHT_WIDTH = 8.0;
		cr->rectangle(get<0>(_nearpos) - HIGHLIGHT_WIDTH/2.0, get<1>(_nearpos) - HIGHLIGHT_WIDTH/2.0, HIGHLIGHT_WIDTH, HIGHLIGHT_WIDTH);
		cr->stroke();
	}

	return true;
}

bool near(point_xy const & p1, point_xy const & p2)
{
	return distance(p1, p2) < 4.0;
}

void example_window::find_nearest_line_point()
{
	_near_line_point = false;

	// ignore newly created points
	if (!_new_line && near(_line_begin, _curpos))
		return;

	for (line const & l : _lines)
	{
		if (near(l.p1, _curpos))
		{
			_near_line_point = true;
			_nearpos = l.p1;
			return;  // onnly one end point can be highlighted
		}

		if (near(l.p2, _curpos))
		{
			_near_line_point = true;
			_nearpos = l.p2;
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
