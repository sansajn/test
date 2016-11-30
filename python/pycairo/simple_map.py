''' vykresly mapu '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
import cairo, math

MAX_MAP_LEVEL = 4

class Example(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self._level = 4
		self._darea = None
		self.init_ui()

	def init_ui(self):
		darea = Gtk.DrawingArea()
		darea.connect('draw', self.on_draw)
		darea.connect('button-press-event', self.on_button_press)
		darea.set_events(Gdk.EventMask.BUTTON_PRESS_MASK)
		self.add(darea)
		self._darea = darea

		self.set_title('simple map render')
		self.resize(800, 600)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, wid, cr):
		map_size = 2**self._level*256
		for x in range(0, 2**self._level):
			for y in range(0, 2**self._level):
				image_path = 'tiles/%d_%d_%d.png' % (self._level, x, y)
				img = cairo.ImageSurface.create_from_png(image_path)
				xoff, yoff = (x*256 + (800-map_size)/2, y*256 + (600-map_size)/2)
				cr.set_source_surface(img, xoff, yoff)
				cr.paint()

	def on_button_press(self, w, e):
		if e.type == Gdk.EventType.BUTTON_PRESS:
			if e.button == 1:  # left mouse button:
				self.zoom_in()
			elif e.button == 3:  # right mouse button
				self.zoom_out()

	def zoom_in(self):
		self._level = min(self._level+1, MAX_MAP_LEVEL)
		self.queue_map_draw()

	def zoom_out(self):
		self._level = max(self._level-1, 0)
		self.queue_map_draw()

	def queue_map_draw(self):
		self._darea.queue_draw()


def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
