#include "playfieldsquare.h"

#include <QtWidgets>
#include <iostream>

PlayFieldSquare::PlayFieldSquare(GameState *gs, const QColor &color, int x,
                                 int y, string path1, string path2,
                                 MainWindow *mw, Logger *l)
{
    this->x = x;
    this->y = y;
    this->color = color;
    gState = gs;
    window = mw;
    log = l;
    setZValue((x + y) % 2);

    if (path1 != "")
    {
        QString p = QString::fromStdString(path1);
        pic.load(p);
        pic = pic.scaledToHeight(100, Qt::SmoothTransformation);
    }
    if (path2 != "")
    {
        QString p2 = QString::fromStdString(path2);
        teamPic.load(p2);
        teamPic = teamPic.scaledToHeight(90, Qt::SmoothTransformation);
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

void PlayFieldSquare::setPath1(string p)
{
    QString pa = QString::fromStdString(p);
    pic.load(pa);
}

void PlayFieldSquare::setPath2(string p)
{
    QString pa = QString::fromStdString(p);
    teamPic.load(pa);
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
    if (!teamPic.isNull())
    {
        painter->drawPixmap(QRect(5,5,90,90), teamPic);
    }
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
    QGraphicsItem::mouseReleaseEvent(event);

    gState->setClickedX(x);
    gState->setClickedY(y);
    //cout << "Coord is (" << x << "," << y << ")\n" << std::flush;
    int result = 1000;
    if (gState->getTurnState() == 1)
        result = gState->turnStateMove();
    else if (gState->getTurnState() == 3)
        result = gState->turnStateChoseTargets();
    else if (gState->getTurnState() == 5)
        result = gState->turnStateExtraMove();

    if (result == 11 || result == 31 || result == 51)
    {
        log->addToErrorLog("Invalid space selection!");
        window->reDrawMenus();
    }
    else if (result == 32 || result == 33 || result == 34)
    {
        log->addToErrorLog("A Major Error occurred! (GameState::turnStateChoseTargets().");
        window->reDrawMenus();
    }
    else if (result == 41)
    {
        window->reColorScene();
    }
    else if (result == 35) // Action was canceled
    {
        window->reDrawMenus();
    }
    else if (result == 20 || result == 40 || result == 60)
    {
        log->addToErrorLog("Invalid turnState error.");
        window->reDrawMenus();
    }
    else if (result == 666)
    {
        qApp->quit();
    }
    else if (result != 0 && result != 1000)
    {
        log->addToErrorLog("An Unknown Error occurred?! Result: " + to_string(result));
        window->reDrawMenus();
    }
    else if (result == 0)
    {
        window->reColorScene();
    }

    update();
}
