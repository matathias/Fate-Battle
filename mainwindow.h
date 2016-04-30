#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "playfieldsquare.h"
#include "GameState.h"

#include <QWidget>
#include <QMainWindow>
#include <QtGui>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QList>

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
    void populateScene(int x, int y);

private slots:
    void open();
    void quit();

private:
    Ui::MainWindow *ui;

    void setupMatrix();
    void startGameState();
    void restartGameState(vector<string> team1, vector<string> team2,
                          Team t1, Team t2, int ascensionLvl);

    void mainSetup();
    void initialSetup();

    QGraphicsScene *scene;

    QAction *openAction;
    QAction *exitAction;
    QPushButton *quitButton;

    QMenu *fileMenu;

    QLabel *name;
    QLabel *icon;
    QLabel *nextServ;

    QTableView *statsTable1;
    QTableView *statsTable2;
    QTableView *statsTable3;
    QTableView *debuffTable;

    GameState *gs;
};

#endif // MAINWINDOW_H
