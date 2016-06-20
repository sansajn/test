'''
pouzitie pera
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
		cr.set_source_rgba(0, 0, 0, 1)
		cr.set_line_width(2)

		cr.set_dash([4.0, 21.0, 2.0])

		cr.move_to(40, 30)
		cr.line_to(250, 30)
		cr.stroke()

		cr.set_dash([14.0, 6.0])

		cr.move_to(40, 50)
		cr.line_to(250, 50)
		cr.stroke()

		cr.set_dash([1.0])

		cr.move_to(40, 70)
		cr.line_to(250, 70)
		cr.stroke()


def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
