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
#include <QSpinBox>
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

    void loadGame();

    void initGameState();

    void quitGame();

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
    void but10();
    void but11();
    void but12();

    void endTurn();
    void cancelAction();

    void loadCBTeam1_0(int index);
    void loadCBTeam1_1(int index);
    void loadCBTeam1_2(int index);
    void loadCBTeam1_3(int index);
    void loadCBTeam1_4(int index);
    void loadCBTeam1_5(int index);
    void loadCBTeam1_6(int index);
    void loadCBTeam1_7(int index);
    void loadCBTeam2_0(int index);
    void loadCBTeam2_1(int index);
    void loadCBTeam2_2(int index);
    void loadCBTeam2_3(int index);
    void loadCBTeam2_4(int index);
    void loadCBTeam2_5(int index);
    void loadCBTeam2_6(int index);
    void loadCBTeam2_7(int index);

    void loadCBTeam1(int index);
    void loadCBTeam2(int index);

    void loadAscLvl(int index);

    void loadFieldWidth(int index);
    void loadFieldLength(int index);

private:
    Ui::MainWindow *ui;

    void setupMatrix();
    void startGameState();
    void restartGameState(vector<string> team1, vector<string> team2,
                          Team t1, Team t2, int ascensionLvl, int fX, int fY);

    void mainSetup();
    void initialSetup();

    // If num is less than 0, returns 0. Otherwise, returns num.
    int capZero(int num);

    void clearLayout(QLayout *l);
    void clearSelections();
    void setStringLists();

    void buttonProcess();

    bool isTeamEmpty(vector<string> team);

    void printErrorLog();
    void printEventLog();

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
    QRadioButton *button10;
    QRadioButton *button11;
    QRadioButton *button12;

    QTableView *statsTable1;
    QTableView *statsTable2;
    QTableView *statsTable3;
    QTableView *debuffTable;

    QHBoxLayout *mainLayout;

    GameState *gs;
    Logger *log;

    // Stuff for the loading dialog
    vector<string> teamOne;
    vector<string> teamTwo;
    QStringList servantNames;
    Team tOne;
    Team tTwo;
    QStringList teamNames;
    QStringList ascLevelNames;
    int ascLevel;
    int fieldWid;
    int fieldLen;
};

#endif // MAINWINDOW_H
