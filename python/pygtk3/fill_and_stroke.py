'''
nakresli vyplneny kruh
source: http://zetcode.com/gfx/pycairo/basicdrawing/
'''

from gi.repository import Gtk, Gdk
import cairo
import math

class Example(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.init_ui()

	def init_ui(self):
		darea = Gtk.DrawingArea()
		darea.connect('draw', self.on_draw)
		self.add(darea)

		self.set_title('Fill & stroke')
		self.resize(230, 150)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, wid, cr):
		cr.set_line_width(9)
		cr.set_source_rgb(0.7, 0.2, 0.0)

		w, h = self.get_size()

		cr.translate(w/2, h/2)
		cr.arc(0, 0, 50, 0, 2*math.pi)
		cr.stroke_preserve()  # preserves shape

		cr.set_source_rgb(0.3, 0.4, 0.6)
		cr.fill()

def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
