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

		self.set_title('transparent colors')
		self.resize(600, 90)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, widget, cr):
		for i in range(1, 11):
			cr.set_source_rgba(0, 0, 1, i*0.1)
			cr.rectangle(50*i, 20, 40, 40)
			cr.fill()

def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
