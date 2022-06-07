// QT GUI wello world sample
#include <QMainWindow>
#include <QApplication>

class main_window : public QMainWindow {
public:
	main_window() : QMainWindow() {}
};

int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	main_window w;
	w.show();
	app.exec();
	return 0;
}
