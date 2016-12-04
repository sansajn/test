'''kreslenie zakladnych tvarou v gtk3 okne'''
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

		self.set_title('basic shapes')
		self.resize(400, 300)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, widget, cr):
		cr.set_source_rgb(0.6, 0.6, 0.6)

		cr.rectangle(20, 20, 120, 80)
		cr.rectangle(180, 20, 80, 80)
		cr.fill()

		cr.arc(330, 60, 40, 0, 2*math.pi)
		cr.fill()

		cr.arc(90, 160, 40, math.pi/2, math.pi)
		cr.fill()

		cr.translate(220, 180)  # draw oval
		cr.scale(1, 0.7)
		cr.arc(0, 0, 50, 0, 2*math.pi)
		cr.fill()


def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
