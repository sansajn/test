// Graphics View Framework hello sample based on *Foundations of Qt Development* book from Chapter 7, p.g. 215.
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

int main(int argc, char * argv[]) {
	QApplication app{argc, argv};
	QGraphicsScene scene{QRect{-50, -50, 400, 200}};

	QGraphicsRectItem * rectItem = scene.addRect(QRectF{-25, 25, 200, 40});
	rectItem->setPen(QPen{Qt::red, 3, Qt::DashDotLine});
	rectItem->setBrush(Qt::gray);

	QGraphicsSimpleTextItem * textItem = scene.addSimpleText("Foundations of Qt");
	textItem->setPos(50, 0);

	QGraphicsEllipseItem * ellipseItem = scene.addEllipse(QRectF{170, 20, 100, 75}, QPen{Qt::darkBlue}, Qt::blue);

	QVector<QPointF> points;
	points << QPointF{10, 10} << QPointF{0, 90} << QPointF{40, 70} << QPointF{80, 110} << QPointF{70, 20};
	QGraphicsPolygonItem * polygonItem = scene.addPolygon(QPolygonF{points}, QPen{Qt::darkGreen}, Qt::yellow);

	QGraphicsView view;
	view.setScene(&scene);
	view.show();

	return app.exec();
}
