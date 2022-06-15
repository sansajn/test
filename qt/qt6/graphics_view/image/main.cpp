// Graphics View Framework image sample with `QGraphicsPixmapItem`.
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

int main(int argc, char * argv[]) {
	QApplication app{argc, argv};

	// load sample image
	QPixmap lena;
	bool const loaded = lena.load("../image/lena.jpg");  // TODO: project setup dependent, change
	assert(loaded);

	QGraphicsScene scene{QRect{0, 0, lena.width(), lena.height()}};

	QGraphicsPixmapItem * image_item = scene.addPixmap(lena);

	QGraphicsView view;
	view.setScene(&scene);
	view.show();

	return app.exec();
}
