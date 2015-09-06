import sys
from PyQt4 import QtGui

def main(args):
	app = QtGui.QApplication(args)
	form = Form()
	form.show()
	app.exec_()


class Form(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self, None)
		self.resize(800, 600)

	def mouseMoveEvent(self, e):
		print 'mouse_move_event'

	def mousePressEvent(self, e):
		print 'mouse_press_event'

	def mouseReleaseEvent(self, e):
		print 'mouse_release_event'

if __name__ == '__main__':
	main(sys.argv)
