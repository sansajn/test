import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class TogglrButtonWindow(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='ToggleButton Demo')
		self.set_border_width(10)

		hbox = Gtk.Box(spacing=6)
		self.add(hbox)

		button = Gtk.ToggleButton('Button 1')
		button.connect('toggled', self.on_button_toggled, '1')
		hbox.pack_start(button, True, True, 0)

		button = Gtk.ToggleButton('B_utton 2', use_underline=True)
		button.set_active(True)
		button.connect('toggled', self.on_button_toggled, '2')
		hbox.pack_start(button, True, True, 0)

	def on_button_toggled(self, button, name):
		if button.get_active():
			state = 'on'
		else:
			state = 'off'
		print('Button', name, 'was turned', state)

win = TogglrButtonWindow()
win.connect('delete-event', Gtk.main_quit)
win.show_all()
Gtk.main()
