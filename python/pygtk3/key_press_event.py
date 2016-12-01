''' stlacenie tlacitka klavesnice '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

class example_window(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='button-press-event test')
		
		self.set_events(Gdk.EventMask.KEY_PRESS_MASK)
		self.connect('key-press-event', self.on_key_press)

		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_key_press(self, widget, event):
		'''widget:Gdk.Widget, event:Gdk.EventKey'''
		if event.type == Gdk.EventType.KEY_PRESS:
			print('key %d pressed -> %s' % (event.keyval, event.string))

def main():
	w = example_window()
	Gtk.main()

if __name__ == '__main__':
	main()
