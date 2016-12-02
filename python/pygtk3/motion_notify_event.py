''' sledovanie pohybu mysi '''
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

class example_window(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title=' test')
		
		self.set_events(Gdk.EventMask.POINTER_MOTION_MASK)
		self.connect('motion-notify-event', self.on_motion_notify)

		self.connect('delete-event', Gtk.main_quit)
		self.show_all()

	def on_motion_notify(self, widget, event):
		''' widget:Gdk.Widget, event:Gdk.EventMotion '''
		print('Local(%d, %d), Screen(%g, %g)' % (event.x, event.y, event.x_root, event.y_root))
		

def main():
	w = example_window()
	Gtk.main()

if __name__ == '__main__':
	main()
