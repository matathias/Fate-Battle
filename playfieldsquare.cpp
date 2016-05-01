#include "playfieldsquare.h"

#include <QtWidgets>
#include <iostream>

PlayFieldSquare::PlayFieldSquare(GameState *gs, const QColor &color, int x,
                                 int y, string path, MainWindow *mw)
{
    this->x = x;
    this->y = y;
    this->color = color;
    this->gState = gs;
    window = mw;
    setZValue((x + y) % 2);

    if (path != "")
    {
        QString p = QString::fromStdString(path);
        pic.load(p);
    }

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

int PlayFieldSquare::getX()
{
    return x;
}

int PlayFieldSquare::getY()
{
    return y;
}

void PlayFieldSquare::setPath(string p)
{
    QString pa = QString::fromStdString(p);
    pic.load(pa);
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

void PlayFieldSquare::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
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
    if (!pic.isNull())
    {
        painter->drawPixmap(QRect(0,0,100,100), pic);
    }
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
    cout << "Beginning of PlayFieldSquare::mouseReleaseEvent().\n" << std::flush;
    QGraphicsItem::mouseReleaseEvent(event);

    gState->setClickedX(x);
    gState->setClickedY(y);
    int result = 1000;
    if (gState->getTurnState() == 1)
        result = gState->turnStateMove();
    else if (gState->getTurnState() == 3)
        result = gState->turnStateChoseTargets();
    else if (gState->getTurnState() == 5)
        result = gState->turnStateExtraMove();

    if (result == 11 || result == 31 || result == 51)
    {
        gState->addToEventLog("Invalid space selection!");
        // It crashes if an invalid space is selected twice??
        window->reDrawMenus();
    }
    else if (result == 32 || result == 33 || result == 34)
    {
        gState->addToEventLog("A Major Error occurred! (turnStateChoseTargets().");
        window->reDrawMenus();
    }
    else if (result != 0 && result != 1000)
    {
        gState->addToEventLog("An Unknown Error occurred?!");
        window->reDrawMenus();
    }
    else if (result == 0)
    {
        window->reColorScene();
    }

    cout << "In PlayFieldSquare::mouseReleaseEvent(), just before update().\n" << std::flush;
    update();
    cout << "End of PlayFieldSquare::mouseReleaseEvent().\n\n" << std::flush;
}
