'''use transparent colors'''
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

		self.set_title('arc')
		self.resize(230, 150)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, widget, cr):
		cr.set_line_width(9)
		cr.set_source_rgb(0.7, 0.2, 0.0)

		cr.translate(230/2, 150/2)  # move to center
		cr.arc(0, 0, 50, 0, 2*math.pi)
		cr.stroke_preserve()

		cr.set_source_rgb(0.3, 0.4, 0.6)
		cr.fill()

def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
