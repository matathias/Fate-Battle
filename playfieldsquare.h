#ifndef PLAYFIELDSQUARE_H
#define PLAYFIELDSQUARE_H

#pragma once

//#include "GameState.h"
#include "mainwindow.h"
#include <QColor>
#include <QGraphicsItem>

class PlayFieldSquare : public QGraphicsItem
{
public:
    PlayFieldSquare(GameState *gs, const QColor &color, int x, int y,
                    string path, MainWindow *mw);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void changeColor(const QColor &color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;

    int getX();
    int getY();
    void setPath(string p);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int x;
    int y;
    QColor color;
    QVector<QPointF> stuff;
    GameState *gState;
    QPixmap pic;
    MainWindow *window;
};

#endif // PLAYFIELDSQUARE_H
