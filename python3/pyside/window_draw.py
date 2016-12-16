import sys, math
from PySide import QtGui

class app_window(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self, None)

	def paintEvent(self, e):
		qp = QtGui.QPainter()
		qp.begin(self)
		qp.drawArc(0,0,50,50,0,16*360)
		qp.end()

def main():
	app = QtGui.QApplication(sys.argv)
	w = app_window()
	w.show()
	app.exec_()


if __name__ == '__main__':
	main()
