import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
import cairo, math

class example(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self)
		self.init_ui()

	def init_ui(self):
		self.connect('draw', self.on_draw)

		self.set_title('Fill & stroke')
		self.resize(230, 150)
		self.set_position(Gtk.WindowPosition.CENTER)
		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_draw(self, wid, cr):
		print('on_draw()')

def main():
	app = example()
	Gtk.main()

if __name__ == '__main__':
	main()
