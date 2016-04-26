#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "playfieldsquare.h"

#include <QWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget //QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void repopulateScene(int x, int y);

private:
    Ui::MainWindow *ui;

    void setupMatrix();
    void populateScene();

    QGraphicsScene *scene;
    QSplitter *h1Splitter;
    QSplitter *h2Splitter;
};

#endif // MAINWINDOW_H
