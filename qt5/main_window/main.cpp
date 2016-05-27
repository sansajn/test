#include <QMainWindow>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QMainWindow w;
	w.setGeometry(0, 0, 800, 600);
	w.show();
	return a.exec();
}
