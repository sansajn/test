// Graphics View Framework input (and custom item) sample based on *Foundations of Qt Development* book from Chapter 7, Listings 7-22, ..., 7-31
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

#include <iostream>
using std::cout;


class HandleItem : public QGraphicsItem {
public:
	enum class HandleRole {CenterHandle, RightHandle, TopHandle};

	HandleItem(QGraphicsItem * item, QGraphicsScene * scene, QColor color,
		HandleRole role = HandleRole::CenterHandle, QList<HandleItem *> other_handles = {});

	void paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) override;  // TODO: use reference there
	QRectF boundingRect() const override;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const & data) override;  // TODO: can we use std::variant there?

private:
	QGraphicsItem * _item;
	HandleRole _role;
	QColor _color;
	QList<HandleItem *> _other_handles;
	bool _pressed;
};

constexpr int default_zvalue = 100;

HandleItem::HandleItem(QGraphicsItem * item, QGraphicsScene * scene, QColor color,
	HandleRole role, QList<HandleItem *> other_handles)
	: _item{item}
	, _role{role}
	, _color{color}
	, _other_handles{other_handles}
	, _pressed{false} {

	assert(item && scene);
	scene->addItem(this);
	setZValue(default_zvalue);
	setFlags(ItemIsMovable|ItemSendsScenePositionChanges);
}

QRectF HandleItem::boundingRect() const {
	assert(_item);
	QPointF point = _item->boundingRect().center();

	switch (_role) {
		case HandleRole::CenterHandle: return QRectF{point - QPointF{5, 5}, QSizeF{10, 10}};
		case HandleRole::RightHandle: {
			point.setX(_item->boundingRect().right());
			return QRectF{point - QPointF{3, 5}, QSizeF{6, 10}};
		}
		case HandleRole::TopHandle: {
			point.setY(_item->boundingRect().top());
			return QRectF{point - QPointF{5, 3}, QSizeF{10, 6}};
		}
	}

	return {};
}

void HandleItem::paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) {
	assert(paint && option && widget);

	paint->setPen(_color);
	paint->setBrush(_color);

	QRectF rect = boundingRect();

	switch (_role) {
		case HandleRole::CenterHandle: {
			paint->drawEllipse(rect);
			break;
		}

		case HandleRole::RightHandle: {
			QVector<QPointF> points;  // TODO: we can use std::vector there
			points << rect.center() + QPointF{3,0} << rect.center() + QPointF{-3, -5} << rect.center() + QPointF{-3, 5};
			paint->drawConvexPolygon(QPolygonF{points});
			break;
		}

		case HandleRole::TopHandle: {
			QVector<QPointF> points;  // TODO: we can use std::vector there
			points << rect.center() + QPointF{0, -3} << rect.center() + QPointF{-5, 3} << rect.center() + QPointF{5, 3};
			paint->drawConvexPolygon(QPolygonF{points});
			break;
		}
	}
}

void HandleItem::mousePressEvent(QGraphicsSceneMouseEvent * event) {
	_pressed = true;
	QGraphicsItem::mousePressEvent(event);
}

void HandleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
	_pressed = false;
	QGraphicsItem::mouseReleaseEvent(event);
}

QVariant HandleItem::itemChange(GraphicsItemChange change, QVariant const & data) {
	assert(_item);
	if (change == ItemPositionChange && _pressed) {
		QPointF const movement = data.toPointF() - pos(),
			center = _item->boundingRect().center();

		switch (_role) {
			case HandleRole::CenterHandle: {
				_item->moveBy(movement.x(), movement.y());
				for (HandleItem * h: _other_handles)
					h->setTransform(QTransform::fromTranslate(movement.x(), movement.y()), true);

				return QGraphicsItem::itemChange(change, pos() + movement);
			}

			case HandleRole::TopHandle: {
				auto move_by = movement;
				if (-2 * movement.y() + _item->sceneBoundingRect().height() <= 5)
					return QGraphicsItem::itemChange(change, pos());

				move_by.setX(0);

				QTransform T;
				T.translate(center.x(), center.y());
				T.scale(1, 1.0 - 2.0 * movement.y() / _item->sceneBoundingRect().height());
				T.translate(-center.x(), -center.y());
				_item->setTransform(T, true);  // combine transformation

				return QGraphicsItem::itemChange(change, pos() + move_by);
			}

			case HandleRole::RightHandle: {
				auto move_by = movement;
				if (2 * movement.x() + _item->sceneBoundingRect().width() <= 5)
					return QGraphicsItem::itemChange(change, pos());

				move_by.setY(0);

				QTransform T;
				T.translate(center.x(), center.y());
				T.scale(1.0 + 2.0 * movement.x() / _item->sceneBoundingRect().width(), 1);
				T.translate(-center.x(), -center.y());
				_item->setTransform(T, true);  // combine transformation

				return QGraphicsItem::itemChange(change, pos() + move_by);
			}
		}
	}

	return QGraphicsItem::itemChange(change, data);
}


int main(int argc, char * argv[]) {
	QApplication app{argc, argv};
	QGraphicsScene scene{QRectF{0, 0, 200, 200}};

	QGraphicsRectItem * rectItem = scene.addRect(QRectF{10, 10, 50, 100});
	QGraphicsEllipseItem * elItem = scene.addEllipse(QRectF{80, 40, 100, 80});

	HandleItem * trh = new HandleItem{rectItem, &scene, Qt::red, HandleItem::HandleRole::TopHandle};
	HandleItem * rrh = new HandleItem{rectItem, &scene, Qt::red, HandleItem::HandleRole::RightHandle};
	HandleItem * crh = new HandleItem{rectItem, &scene, Qt::red, HandleItem::HandleRole::CenterHandle,
		QList<HandleItem *>{} << trh << rrh};  // TODO: initializer-list support?

	HandleItem * teh = new HandleItem{elItem, &scene, Qt::green, HandleItem::HandleRole::TopHandle};
	HandleItem * reh = new HandleItem{elItem, &scene, Qt::green, HandleItem::HandleRole::RightHandle};
	HandleItem * ceh = new HandleItem{elItem, &scene, Qt::green, HandleItem::HandleRole::CenterHandle,
		QList<HandleItem *>() << teh << reh};  // TODO: initializer-list support?

	QGraphicsView view;
	view.setBackgroundBrush(Qt::white);
	view.setScene(&scene);
	view.show();

	return app.exec();
}
