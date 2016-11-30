''' vykreslenie obrazka '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
import cairo, math

image_path = 'volley.png'

class Example(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.init_ui()

	def init_ui(self):
		darea = Gtk.DrawingArea()
		darea.connect('draw', self.on_draw)
		self.add(darea)

		self.set_title('Fill & stroke')
		self.resize(600, 850)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, wid, cr):
		surf = cairo.ImageSurface.create_from_png(image_path)
		cr.set_source_surface(surf, 0, 0)
		cr.paint()

def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
