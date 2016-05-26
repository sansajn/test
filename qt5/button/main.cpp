#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QPushButton button{"Hello!"};
	button.show();
	return a.exec();
}
