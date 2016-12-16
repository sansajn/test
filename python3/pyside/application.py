# -*- coding: utf-8 -*-
import sys
from PySide import QtCore, QtGui

class window(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self, None)

def main(args):
	app = QtGui.QApplication(args)
	w = window()
	w.show()
	app.exec_()

if __name__ == '__main__':
	main(sys.argv)
