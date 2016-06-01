# grid kontajner, kapitola 5.2.1
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class MyWindow(Gtk.Window):
	def __init__(self):
		Gtk.Window.__init__(self, title='Hello World')
		
		grid = Gtk.Grid()
		self.add(grid)

		button1 = Gtk.Button('button 1')
		button2 = Gtk.Button('button 2')
		button3 = Gtk.Button('button 3')
		button4 = Gtk.Button('button 4')
		button5 = Gtk.Button('button 5')
		button6 = Gtk.Button('button 6')
		
		grid.add(button1)
		grid.attach(button2, 1, 0, 2, 1)
		grid.attach_next_to(button3, button1, Gtk.PositionType.BOTTOM, 1, 2)
		grid.attach_next_to(button4, button3, Gtk.PositionType.RIGHT, 2, 1)
		grid.attach(button5, 1, 2, 1, 1)
		grid.attach_next_to(button6, button5, Gtk.PositionType.RIGHT, 1, 1)
		
		
win = MyWindow()
win.connect('delete-event', Gtk.main_quit)
win.show_all()
Gtk.main()
