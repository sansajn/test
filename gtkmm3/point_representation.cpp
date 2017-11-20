// reprezentacia bodu pri praci s gtkmm
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/format.hpp>
#include <gtkmm.h>
#include <gdkmm.h>
#include <gdk/gdk.h>

namespace bg = boost::geometry;

using bg::get;
using point_xy = bg::model::d2::point_xy<double>;


class example_window : public Gtk::Window
{
public:
	example_window();

private:
	bool on_motion_notify_event(GdkEventMotion * event) override;
	bool on_timeout();

	point_xy _cursor;
};

example_window::example_window()
{
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &example_window::on_timeout), 100);
	add_events(Gdk::POINTER_MOTION_MASK);
}

bool example_window::on_motion_notify_event(GdkEventMotion * event)
{
	_cursor = point_xy{event->x, event->y};
	return true;  // do not propagate
}

bool example_window::on_timeout()
{
	set_title(boost::str(boost::format{"cursor: (%1%, %2%)"} % get<0>(_cursor) % get<1>(_cursor)));
	return true;  // ?
}


int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "test.gtkmm3.point_representation");

	example_window w;
	w.set_default_size(400, 200);

	return app->run(w);
}


