#include <QCoreApplication>
#include <QTimer>
#include "foo.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	foo f;
	QTimer foo_timer{&a};
	QObject::connect(&foo_timer, SIGNAL(timeout()), &f, SLOT(call()));
	foo_timer.start(500);

	return a.exec();
}
