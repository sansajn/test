// Graphics View Framework transform sample based on *Foundations of Qt Development* book from Chapter 7, Listings 7-20, 7-21
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>


QGraphicsItem * createItem(int x, QGraphicsScene & scene);

int main(int argc, char * argv[]) {
	QApplication app{argc, argv};
	QGraphicsScene scene{QRect{0, 0, 1000, 200}};

	QGraphicsItem * item1 = createItem(0, scene);

	QGraphicsItem * item2 = createItem(200, scene);
	QTransform T2;
	T2.translate(300, 100);
	T2.rotate(30);
	T2.translate(-300, -100);
	item2->setTransform(T2);

	QGraphicsItem * item3 = createItem(400, scene);
	QTransform T3;
	T3.translate(500, 100);
	T3.scale(.5, .7);
	T3.translate(-500, -100);
	item3->setTransform(T3);

	QGraphicsItem * item4 = createItem(600, scene);
	QTransform T4;
	T4.translate(700, 100);
	T4.shear(.1, .3);
	T4.translate(-700, -100);
	item4->setTransform(T4);

	QGraphicsItem * item5 = createItem(800, scene);
	QTransform T5;
	T5.translate(900, 100);
	T5.scale(.5, .7);
	T5.rotate(30);
	T5.shear(.1, .3);
	T5.translate(-900, -100);
	item5->setTransform(T5);

	QGraphicsView view;
	view.setScene(&scene);
	view.show();

	return app.exec();
}


QGraphicsItem * createItem(int x, QGraphicsScene & scene) {
	QGraphicsRectItem * rectItem = scene.addRect(QRectF(x+40, 40, 120, 120), QPen{Qt::black}, Qt::gray);

	QGraphicsRectItem * innerRectItem = new QGraphicsRectItem{QRectF(x+50, 50, 45, 100), rectItem};
	innerRectItem->setPen(QPen{Qt::black});
	innerRectItem->setBrush(Qt::white);

	QGraphicsEllipseItem * ellipseItem = new QGraphicsEllipseItem{QRectF(x+105, 50, 45, 100), rectItem};
	ellipseItem->setPen(QPen{Qt::black});
	ellipseItem->setBrush(Qt::white);

	return rectItem;
}
