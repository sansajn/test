'''use colors'''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
import cairo, math

class Example(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.init_ui()

	def init_ui(self):
		darea = Gtk.DrawingArea()
		darea.connect('draw', self.on_draw)
		self.add(darea)

		self.set_title('colors')
		self.resize(360, 100)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, widget, cr):
		cr.set_source_rgb(0.2, 0.23, 0.9)
		cr.rectangle(10, 15, 90, 60)
		cr.fill()

		cr.set_source_rgb(0.9, 0.1, 0.1)
		cr.rectangle(130, 15, 90, 60)
		cr.fill()

		cr.set_source_rgb(0.4, 0.9, 0.4)
		cr.rectangle(250, 15, 90, 60)
		cr.fill()


def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
