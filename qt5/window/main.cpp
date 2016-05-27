#include <QApplication>
#include <QWindow>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QWindow w{};
	w.setGeometry(0, 0, 800, 600);
	w.setTitle("Custom window title");
	w.show();

	return a.exec();
}
