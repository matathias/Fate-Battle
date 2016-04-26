#include "playfieldsquare.h"

#include <QtWidgets>
#include <iostream>

PlayFieldSquare::PlayFieldSquare(/*GameState *gs,*/ const QColor &color, int x, int y)
{
    this->x = x;
    this->y = y;
    this->color = color;
    //this->gState = gs;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF PlayFieldSquare::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

QPainterPath PlayFieldSquare::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 100, 100);
    return path;
}

void PlayFieldSquare::changeColor(const QColor &color)
{
    this->color = color;
}

void PlayFieldSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(0, 0, 100, 100));
    painter->setBrush(b);
}

void PlayFieldSquare::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void PlayFieldSquare::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void PlayFieldSquare::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    // on mouse release, send (x,y) to game class to handle the click
    // divide x,y by grid size to obtain grid coordinate, then do stuff
    QPointF loc = event->lastScenePos();
    std::cout << loc.x() / 100 << " " << loc.y() / 100 << "\n";
    update();
}
