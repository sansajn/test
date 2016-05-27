#include <QApplication>
#include <QMainWindow>
#include <QPainter>

class custom_widget : public QWidget
{
public:
	void paintEvent(QPaintEvent * event);
};

void custom_widget::paintEvent(QPaintEvent * event)
{
	QPainter p{this};
	p.fillRect(0, 0, width(), height(), QBrush{QColor{250, 0, 0}});
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	custom_widget * canvas = new custom_widget;

	QMainWindow w;
	w.setCentralWidget(canvas);
	w.show();

	return a.exec();
}
