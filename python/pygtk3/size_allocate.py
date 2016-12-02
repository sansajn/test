''' velkost okna '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

class example_window(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='size-allocate test')
		
		self.connect('size-allocate', self.on_size_allocate)

		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_size_allocate(self, widget, allocation):
		''' widget:Gdk.Widget, allocation:Gdk.Rectangle '''
		print('(%d, %d, %d, %d)' % (allocation.x, allocation.y, allocation.width, allocation.height))

def main():
	w = example_window()
	Gtk.main()

if __name__ == '__main__':
	main()
