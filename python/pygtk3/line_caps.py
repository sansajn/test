'''
zakoncenia ciar
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
		cr.set_line_width(12)

		cr.set_line_cap(cairo.LINE_CAP_BUTT)
		cr.move_to(30, 50)
		cr.line_to(150, 50)
		cr.stroke()

		cr.set_line_cap(cairo.LINE_CAP_ROUND)
		cr.move_to(30, 90)
		cr.line_to(150, 90)
		cr.stroke()

		cr.set_line_cap(cairo.LINE_CAP_SQUARE)
		cr.move_to(30, 130)
		cr.line_to(150, 130)
		cr.stroke()

		cr.set_line_width(1.5)

		cr.move_to(30, 35)
		cr.line_to(30, 145)
		cr.stroke()

		cr.move_to(150, 35)
		cr.line_to(150, 145)
		cr.stroke()

		cr.move_to(155, 35)
		cr.line_to(155, 145)
		cr.stroke()


def main():
	app = Example()
	Gtk.main()

if __name__ == '__main__':
	main()
