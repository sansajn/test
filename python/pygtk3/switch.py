# test vypinaca
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class SwitcherWindow(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='Switch Demo')
		self.set_border_width(10)

		hbox = Gtk.Box(spacing=6)
		self.add(hbox)

		switch = Gtk.Switch()
		switch.connect('notify::active', self.on_switch_activated)
		switch.set_active(False)
		hbox.pack_start(switch, True, True, 0)

		switch = Gtk.Switch()
		switch.connect('notify::active', self.on_switch_activated)
		switch.set_active(True)
		hbox.pack_start(switch, True, True, 0)

	def on_switch_activated(self, switch, gparam):
		if switch.get_active():
			state = 'on'
		else:
			state = 'off'
		print('switch was turned', state)

w = SwitcherWindow()
w.connect('delete-event', Gtk.main_quit)
w.show_all()
Gtk.main()
