import sys
from PyQt4 import QtGui, QtCore

def main(args):
	app = QtGui.QApplication(args)
	form = Form()
	form.show()
	app.exec_()


class Form(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self, None)
		self.resize(800, 600)

	def paintEvent(self, e):
		painter = QtGui.QPainter()
		painter.begin(self)

		self._draw_circle_mark(painter)

		painter.end()

	def _draw_circle_mark(self, painter):
		# red with black border
		red_brush = QtGui.QBrush(QtGui.QColor(222, 0, 0))

		black_solid_pen = QtGui.QPen(QtGui.QColor(0, 0, 0))
		black_solid_pen.setStyle(QtCore.Qt.SolidLine)
		black_solid_pen.setWidth(5)

		painter.setBrush(red_brush)
		painter.setPen(black_solid_pen)
		painter.drawEllipse(50, 50, 10, 10)




	
if __name__ == '__main__':
	main(sys.argv)
