#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include <gdkmm/general.h>
#include <cairomm/context.h>
#include <giomm/resource.h>

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();

private:
	bool on_draw(Cairo::RefPtr<Cairo::Context> const & cr) override;

	Glib::RefPtr<Gdk::Pixbuf> _image;
};

MyArea::MyArea()
{
	try
	{
		_image = Gdk::Pixbuf::create_from_file("data/tile.png", 256, 256);
	}
	catch (Gio::ResourceError const & e)
	{
		std::cerr << "ResourceError: " << e.what() << std::endl;
	}
	catch (Gdk::PixbufError const & e)
	{
		std::cerr << "PixbufError:" << e.what() << std::endl;
	}

	if (_image)
		set_size_request(_image->get_width()/2, _image->get_height()/2);
}

bool MyArea::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
	if (!_image)
		return false;

	Gtk::Allocation allocation = get_allocation();
	int const width = allocation.get_width();
	int const height = allocation.get_height();

	Gdk::Cairo::set_source_pixbuf(cr, _image,
		(width - _image->get_width())/2, (height - _image->get_height())/2);
	cr->paint();

	return true;
}

int main(int argc, char * argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	Gtk::Window win;
	win.set_title("DrawingArea");
	win.set_default_size(300, 200);

	MyArea area;
	win.add(area);
	area.show();

	return app->run(win);
}
