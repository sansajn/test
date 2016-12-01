''' stlacenie tlacitka mysi '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

class example_window(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='button-press-event test')
		
		self.set_events(Gdk.EventMask.BUTTON_PRESS_MASK)
		self.connect('button-press-event', self.on_button_press)

		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_button_press(self, widget, event):
		'''widget:Gdk.Widget, event:Gdk.EventButton'''
		if event.type == Gdk.EventType.BUTTON_PRESS:
			if event.button == 1:
				print('left mouse button pressed')
			elif event.button == 3:
				print('right mouse button pressed')
			else:
				print('some (mouse) button pressed')
		else:
			print('some button pressed')

def main():
	w = example_window()
	Gtk.main()

if __name__ == '__main__':
	main()
