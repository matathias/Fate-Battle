#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "GameState.h"
#include "logger.h"

#include <QWidget>
#include <QMainWindow>
#include <QtGui>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QList>
#include <QRadioButton>
#include <QLayout>
#include <QGraphicsSceneEvent>
#include <typeinfo>

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
    void reColorScene();
    void reDrawMenus();
    GameState *getGameState();

public slots:
    void redrawEverything();

private slots:
    void open();
    void quit();

    void but1();
    void but2();
    void but3();
    void but4();
    void but5();
    void but6();
    void but7();
    void but8();
    void but9();

    void endTurn();
    void cancelAction();

private:
    Ui::MainWindow *ui;

    void setupMatrix();
    void startGameState();
    void restartGameState(vector<string> team1, vector<string> team2,
                          Team t1, Team t2, int ascensionLvl);

    void mainSetup();
    void initialSetup();

    // If num is less than 0, returns 0. Otherwise, returns num.
    int capZero(int num);

    void clearLayout(QLayout *l);

    void buttonProcess();

    int eventLogSize;

    QGraphicsScene *scene;

    QAction *openAction;
    QAction *exitAction;
    QPushButton *quitButton;

    QMenu *fileMenu;

    QLabel *errorLabel;
    QLabel *name;
    QLabel *teamName;
    QLabel *icon;
    QLabel *nextServ;
    QLabel *nextServTeam;

    QRadioButton *button1;
    QRadioButton *button2;
    QRadioButton *button3;
    QRadioButton *button4;
    QRadioButton *button5;
    QRadioButton *button6;
    QRadioButton *button7;
    QRadioButton *button8;
    QRadioButton *button9;

    QTableView *statsTable1;
    QTableView *statsTable2;
    QTableView *statsTable3;
    QTableView *debuffTable;

    QHBoxLayout *mainLayout;

    GameState *gs;
    Logger *log;
};

#endif // MAINWINDOW_H
