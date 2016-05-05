#pragma once

#include "mainwindow.h"
#include "view.h"
#include "ui_mainwindow.h"
#include "Servants/servanttest.h"
#include "playfieldsquare.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QListView>
#include <QProgressBar>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    // Instantiate the gameState with default parameters, then pass the
    // gameState to populateScene so that the playFieldSquares can have a
    // pointer to it

    startGameState();
    populateScene(gs->getFieldWidth(), gs->getFieldLength());
}

MainWindow::~MainWindow()
{
    delete ui;
}

GameState *MainWindow::getGameState()
{
    return gs;
}

void MainWindow::reDrawMenus()
{
    mainSetup();
}

void MainWindow::mainSetup()
{
    clearLayout(layout());
    delete layout();

    // Setup the play field display
    View *gameField = new View("Game Field");
    gameField->view()->setScene(scene);

    // Setup the quit button
    quitButton = new QPushButton(tr("Quit"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

    QLabel *seperator = new QLabel(this);
    //seperator->setText(" ");
    seperator->setFrameStyle(QFrame::HLine);

    // Setup the Error warning
    errorLabel = new QLabel(this);
    vector<string> errLog = log->getErrorLog();
    string lab = "";
    if (errLog.size()==0 || log->getErrorNum() != log->getEventNum())
        lab = "\nNo Errors.\n";
    else
        lab = "\n" + errLog.back() + "\n";
    errorLabel->setText(QString::fromStdString(lab));
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);

    /* Servant name and icon */
    QVBoxLayout *nameIcon = new QVBoxLayout;
    QString nI = QString::fromStdString(gs->getCurrentServantType());
    QString tN = QString::fromStdString(gs->getCurrentServantType() +
                                        "\nTeam: " + gs->getCurrentServantTeam());
    name = new QLabel(this);
    name->setText(tN);
    name->setAlignment(Qt::AlignCenter);
    name->setFrameStyle(QFrame::Box | QFrame::Sunken);

    icon = new QLabel(this);
    icon->setFrameStyle(QFrame::Panel);
    nI = QString::fromStdString(gs->getCurrentServant()->getServantIcon());
    QPixmap ic;
    bool result = ic.load(nI);
    icon->setPixmap(ic);
    if (!result)
        icon->setText("Image failed to load");
    icon->setAlignment(Qt::AlignCenter);
    nameIcon->addWidget(icon);
    nameIcon->addWidget(name);
    //nameIcon->addWidget(teamName);

    /* Servant stats Widget */
    // HP and MP
    QLabel *hplab = new QLabel(this);
    QLabel *mplab = new QLabel(this);
    QLabel *hpstat = new QLabel(this);
    QLabel *mpstat = new QLabel(this);
    QString hp = QString::fromStdString(
                    to_string(gs->getCurrentServant()->getCurrHP()) + "/" +
                    to_string(gs->getCurrentServant()->getMaxHP()));
    QString mp = QString::fromStdString(
                    to_string(gs->getCurrentServant()->getCurrMP()) + "/" +
                    to_string(gs->getCurrentServant()->getMaxMP()));
    hplab->setText("HP:");
    hplab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mplab->setText("MP:");
    mplab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hpstat->setText(hp);
    hpstat->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mpstat->setText(mp);
    mpstat->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QProgressBar *hpBar = new QProgressBar(this);
    hpBar->setRange(0, gs->getCurrentServant()->getMaxHP());
    hpBar->setValue(gs->getCurrentServant()->getCurrHP());
    hpBar->setTextVisible(false);
    QProgressBar *mpBar = new QProgressBar(this);
    mpBar->setRange(0, gs->getCurrentServant()->getMaxMP());
    mpBar->setValue(gs->getCurrentServant()->getCurrMP());
    mpBar->setTextVisible(false);

    // Movement
    QLabel *movLab = new QLabel(this);
    movLab->setText(QString::fromStdString("Movement Range: " +
                                 to_string(gs->getCurrentServant()->getMov())));
    movLab->setAlignment(Qt::AlignCenter);
    movLab->setFrameStyle(QFrame::Box | QFrame::Sunken);

    // Attack, Hit rate, critical rate, and evasion
    QLabel *extraStats = new QLabel(this);
    extraStats->setText(QString::fromStdString("Accuracy: " +
                                to_string(gs->getCurrentServant()->getHitRate())
                                               + "\nEvasion: " +
                                to_string(gs->getCurrentServant()->getEvade()[0])
                                               + "\nCritical Rate: " +
                                to_string(gs->getCurrentServant()->getCriticalRate())));
    extraStats->setAlignment(Qt::AlignCenter);
    extraStats->setFrameStyle(QFrame::Box | QFrame::Sunken);

    // Other Stats
    QLabel *atkStats = new QLabel(this);
    QLabel *defStats = new QLabel(this);
    QString atk = QString::fromStdString("STR: " +
                                to_string(gs->getCurrentServant()->getStr())
                                + "\nMAG: " +
                                to_string(gs->getCurrentServant()->getMag())
                                + "\nSKL: " +
                                to_string(gs->getCurrentServant()->getSkl()));
    QString def = QString::fromStdString("SPD: " +
                                to_string(gs->getCurrentServant()->getSpd())
                                + "\nLUK: " +
                                to_string(gs->getCurrentServant()->getLuk())
                                + "\nDEF: " +
                                to_string(gs->getCurrentServant()->getDef())
                                + "\nRES: " +
                                to_string(gs->getCurrentServant()->getRes()));
    atkStats->setText(atk);
    defStats->setText(def);
    atkStats->setAlignment(Qt::AlignRight | Qt::AlignTop);
    defStats->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    atkStats->setFrameStyle(QFrame::Box | QFrame::Sunken);
    defStats->setFrameStyle(QFrame::Box | QFrame::Sunken);

    QHBoxLayout *hpBox = new QHBoxLayout;
    hpBox->addWidget(hplab);
    hpBox->addWidget(hpBar);
    hpBox->addWidget(hpstat);
    QHBoxLayout *mpBox = new QHBoxLayout;
    mpBox->addWidget(mplab);
    mpBox->addWidget(mpBar);
    mpBox->addWidget(mpstat);
    QVBoxLayout *hpmpBox = new QVBoxLayout;
    hpmpBox->addLayout(hpBox);
    hpmpBox->addLayout(mpBox);

    QHBoxLayout *otherStats = new QHBoxLayout;
    otherStats->addWidget(atkStats);
    otherStats->addWidget(defStats);

    QVBoxLayout *allStats = new QVBoxLayout;
    allStats->addLayout(hpmpBox);
    allStats->addWidget(movLab);
    allStats->addWidget(extraStats);
    allStats->addLayout(otherStats);

    /* Servant Debuff List Widget */
    QLabel *debLabel = new QLabel(this);
    debLabel->setText("----- Status Effects -----");
    debLabel->setAlignment(Qt::AlignCenter);
    vector<Debuff*> deb = gs->getCurrentServant()->getDebuffs();
    int numDebuffs = deb.size();
    QStandardItemModel *debuffTab;
    if (numDebuffs == 0)
    {
        debuffTab = new QStandardItemModel(1,1,this);
        QStandardItem *debEntry = new QStandardItem(QString("No Debuffs!"));
        debuffTab->setItem(0,0,debEntry);
    }
    else
    {
        debuffTab = new QStandardItemModel(numDebuffs, 2, this);
        debuffTab->setHorizontalHeaderItem(0, new QStandardItem(
                                               QString("Name")));
        debuffTab->setHorizontalHeaderItem(1, new QStandardItem(
                                               QString("Description")));
        for (int i = 0; i < numDebuffs; i++)
        {
            QString debName = QString::fromStdString(deb[i]->getDebuffName());
            QString debDes = QString::fromStdString(deb[i]->getDebuffDescrip());
            QStandardItem *dN = new QStandardItem(debName);
            QStandardItem *dD = new QStandardItem(debDes);
            debuffTab->setItem(numDebuffs,0,dN);
            debuffTab->setItem(numDebuffs,1,dD);
        }
    }

    debuffTable = new QTableView;
    debuffTable->setModel(debuffTab);
    debuffTable->resizeColumnsToContents();

    QVBoxLayout *debuffLayout = new QVBoxLayout;
    debuffLayout->addWidget(debLabel);
    debuffLayout->addWidget(debuffTable);


    /* Servant Action List Widget */
    QVBoxLayout *actionList = new QVBoxLayout;
    vector<string> actList = gs->getActionList();
    vector<int> actListCost = gs->getActionMPCosts();

    for (unsigned int i = 0; i < actList.size(); i++)
    {
        QString text = QString::fromStdString(actList[i] + " | \tMP Cost: " +
                                              to_string(actListCost[i]));
        switch(i)
        {
            case 0:
                button1 = new QRadioButton(text, this);
                connect(button1, SIGNAL(clicked()), this, SLOT(but1()));
                actionList->addWidget(button1);
                break;
            case 1:
                button2 = new QRadioButton(text, this);
                connect(button2, SIGNAL(clicked()), this, SLOT(but2()));
                actionList->addWidget(button2);
                break;
            case 2:
                button3 = new QRadioButton(text, this);
                connect(button3, SIGNAL(clicked()), this, SLOT(but3()));
                actionList->addWidget(button3);
                break;
            case 3:
                button4 = new QRadioButton(text, this);
                connect(button4, SIGNAL(clicked()), this, SLOT(but4()));
                actionList->addWidget(button4);
                break;
            case 4:
                button5 = new QRadioButton(text, this);
                connect(button5, SIGNAL(clicked()), this, SLOT(but5()));
                actionList->addWidget(button5);
                break;
            case 5:
                button6 = new QRadioButton(text, this);
                connect(button6, SIGNAL(clicked()), this, SLOT(but6()));
                actionList->addWidget(button6);
                break;
            case 6:
                button7 = new QRadioButton(text, this);
                connect(button7, SIGNAL(clicked()), this, SLOT(but7()));
                actionList->addWidget(button7);
                break;
            case 7:
                button8 = new QRadioButton(text, this);
                connect(button8, SIGNAL(clicked()), this, SLOT(but8()));
                actionList->addWidget(button8);
                break;
            case 8:
                button9 = new QRadioButton(text, this);
                connect(button9, SIGNAL(clicked()), this, SLOT(but9()));
                actionList->addWidget(button9);
                break;
            default:
                // there's an extra action?? there shouldn't be!
                break;
        }
    }

    QLabel *a = new QLabel(this);
    a->setText("----- Actions -----");
    a->setAlignment(Qt::AlignCenter);

    QVBoxLayout *wholeActionList = new QVBoxLayout;
    wholeActionList->addWidget(a);
    wholeActionList->addLayout(actionList);

    if (gs->getTurnState() == 2)
    {
        QPushButton *cancelButton = new QPushButton(tr("Undo Move"));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));
        wholeActionList->addWidget(cancelButton);
    }
    else if (gs->getTurnState() == 3)
    {
        QPushButton *cancelButton = new QPushButton(tr("Choose Different Action"));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));
        wholeActionList->addWidget(cancelButton);
    }

    QPushButton *endTurnButton = new QPushButton(tr("End Turn"));
    connect(endTurnButton, SIGNAL(clicked()), this, SLOT(endTurn()));
    wholeActionList->addWidget(endTurnButton);

    /* Event Log Widget */
    vector<string> eL = log->getEventLog();
    QList<QString> eventLog;
    for (int i = ((int) eL.size())-1; i >= 0; i--)
    {
        eventLog.append(QString::fromStdString(eL[i]));
    }
    if (eL.size() == 0)
    {
        eventLog.append("No events have occured!");
    }
    QStringList evLog(eventLog);
    QStringListModel *evLogW = new QStringListModel(this);
    evLogW->setStringList(evLog);
    QListView *evLogWid = new QListView;
    evLogWid->setModel(evLogW);
    QLabel *evLogLabel = new QLabel(this);
    evLogLabel->setText("----- Event Log -----");
    evLogLabel->setAlignment(Qt::AlignCenter);

    /* Next Servant Widget */
    nextServ = new QLabel(this);
    QString nS = QString::fromStdString("Next Player: " +
                                        gs->peekNextServant()->getName() +
                                        "\nNext Player's Team: " +
                                        gs->peekNextServant()->getTeamName());
    nextServ->setText(nS);
    nextServ->setAlignment(Qt::AlignCenter);
    nextServ->setFrameStyle(QFrame::Box | QFrame::Sunken);

    // Setup the layouts
    nameIcon->addLayout(allStats);
    nameIcon->addLayout(debuffLayout);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(quitButton);
    layout2->addWidget(gameField);
    layout2->addWidget(errorLabel);
    //layout2->addWidget(errorLabel);

    QVBoxLayout *rightMost = new QVBoxLayout;
    rightMost->addWidget(nextServ);
    rightMost->addLayout(wholeActionList);
    rightMost->addWidget(evLogLabel);
    rightMost->addWidget(evLogWid);

    QHBoxLayout *playerInformation = new QHBoxLayout;
    playerInformation->addLayout(nameIcon);
    playerInformation->addLayout(rightMost);

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(playerInformation);

    setLayout(mainLayout);

    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
}

void MainWindow::populateScene(int w, int l)
{
    scene = new QGraphicsScene;

    // Populate scene
    int xx = 0;
    int nitems = 0;
    for (int i = 0; i < 100 * w; i += 100) {
        int yy = 0;
        for (int j = 0; j < 100 * l; j += 100) {

            // Determine the item's color based on whether or not a Servant is
            // there, and on what selection the player is currently making
            //     If Servant: show Servant Icon
            //     If No selection: gray
            //     If Selection for move: blue
            //     If selection for attack: red
            int red = 150;
            int green = 150;
            int blue = 150;
            if (gs->getTurnState() == 3 && gs->isSpaceSelection(xx,yy))
            {
                red = 255;
            }
            else if ((gs->getTurnState() == 1 || gs->getTurnState() == 5) &&
                     gs->isSpaceMove(xx,yy))
            {
                blue = 255;
            }
            else if (gs->isSpaceRealityMarble(xx,yy))
            {
                green = 0;
                blue = 0;
            }
            else if (gs->isSpaceDebuff(xx,yy))
            {
                Team spaceTeam = gs->spaceDebuffTeam(xx,yy);
                if (spaceTeam == Alpha)
                {
                    blue = 100;
                    red = 0;
                    green = 0;
                }
                else if (spaceTeam == Omega)
                {
                    red = 100;
                    blue = 0;
                    green = 0;
                }
                else
                {
                    red = 0;
                    green = 100;
                    blue = 0;
                }
            }
            string imgPath = "";
            string tPath = "";
            Servant* spaceServ = gs->isSpaceServant(xx,yy);
            if (spaceServ != NULL && !gs->isServantDead(spaceServ))
            {
                imgPath = spaceServ->getServantIcon();
                tPath = spaceServ->getTeamIcon();
            }
            QColor color(red, green, blue, 255);
            QGraphicsItem *item = new PlayFieldSquare(gs, color, xx, yy,
                                                      imgPath, tPath, this, log);

            item->setPos(QPointF(i, j));
            scene->addItem(item);

            ++nitems;
            ++yy;
        }
        ++xx;
    }

    mainSetup();
}

void MainWindow::reColorScene()
{
    // Get the items in the scene
    QList<QGraphicsItem*> allItems = scene->items();
    for (int i = 0; i < allItems.size(); i++)
    {
        PlayFieldSquare* temp = dynamic_cast<PlayFieldSquare*>(allItems[i]);
        int x = temp->getX();
        int y = temp->getY();

        int red = 150;
        int green = 150;
        int blue = 150;
        if (gs->getTurnState() == 3 && gs->isSpaceSelection(x,y))
        {
            blue = 0;
            green = 0;
            red = 255;
        }
        else if ((gs->getTurnState() == 1 || gs->getTurnState() == 5) &&
                 gs->isSpaceMove(x,y))
        {
            blue = 255;
        }
        else if (gs->isSpaceRealityMarble(x,y))
        {
            green = 50;
            blue = 50;
            red = 50;
        }
        else if (gs->isSpaceDebuff(x,y))
        {
            Team spaceTeam = gs->spaceDebuffTeam(x,y);
            if (spaceTeam == Alpha)
            {
                blue = 100;
                red = 0;
                green = 0;
            }
            else if (spaceTeam == Omega)
            {
                red = 100;
                blue = 0;
                green = 0;
            }
            else
            {
                red = 0;
                green = 100;
                blue = 0;
            }
        }
        string imgPath = "";
        string tPath = "";
        Servant* spaceServ = gs->isSpaceServant(x,y);
        if (spaceServ != NULL && !gs->isServantDead(spaceServ))
        {
            imgPath = spaceServ->getServantIcon();
            tPath = spaceServ->getTeamIcon();
        }
        QColor color(red, green, blue, 255);
        temp->changeColor(color);
        temp->setPath1(imgPath);
        temp->setPath2(tPath);
    }

    mainSetup();
}

// This function is causing so many problems
// Commenting out the recursive part allows the program to not crash, but the
//     ui is not properly cleared.
// Leaving the recursive part in properly clears the ui, but makes the program
//     crash after a handful of actions. Maybe it's "accidentally" deleting
//     scene or something?
void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while (layout != NULL && (item = layout->takeAt(0)))
    {
        if (QWidget* widget = item->widget())
        {
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
        {
            clearLayout(childLayout);
        }
        delete item;
    }

    //cout << "deleting layout\n" << std::flush;
    //delete layout;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                       tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        // Parse file to find the participating servants and other data
        file.close();
    }
}

void MainWindow::quit()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Final Fate"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}

void MainWindow::buttonProcess()
{
    int result = gs->turnStateChoseAction();

    if (result == 30)
        log->addToErrorLog("Cannot choose action at this moment.");
    else if (result == 21)
        log->addToErrorLog("Invalid choice.");
    else if (result == 22)
        log->addToErrorLog("Not enough MP!"); // Pop up message box?

    reColorScene();
}

void MainWindow::but1()
{
    gs->setChosenAction(0);
    buttonProcess();
}
void MainWindow::but2()
{
    gs->setChosenAction(1);
    buttonProcess();
}
void MainWindow::but3()
{
    gs->setChosenAction(2);
    buttonProcess();
}
void MainWindow::but4()
{
    gs->setChosenAction(3);
    buttonProcess();
}
void MainWindow::but5()
{
    gs->setChosenAction(4);
    buttonProcess();
}
void MainWindow::but6()
{
    gs->setChosenAction(5);
    buttonProcess();
}
void MainWindow::but7()
{
    gs->setChosenAction(6);
    buttonProcess();
}
void MainWindow::but8()
{
    gs->setChosenAction(7);
    buttonProcess();
}
void MainWindow::but9()
{
    gs->setChosenAction(8);
    buttonProcess();
}

void MainWindow::endTurn()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Final Fate"));
    messageBox.setText(tr("Do you really wish to end your turn?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        int result = gs->endTurnProcess();
        if (result == 666)
            qApp->quit();
        else
            reColorScene();
    }
}

void MainWindow::cancelAction()
{
    if (gs->getTurnState() == 2)
    {
        log->addToEventLog("Undid Move Action.");
        gs->prevTurnState();
        reColorScene();
    }
    else if (gs->getTurnState() == 3)
    {
        log->addToEventLog("Undid Action Choice.");
        gs->prevTurnState();
        reColorScene();
    }
}

void MainWindow::redrawEverything()
{
    populateScene(gs->getFieldWidth(), gs->getFieldLength());
}

void MainWindow::startGameState()
{
    Servant *first = new ServantTest(1, Alpha, log);
    Servant *second = new ServantTest(1, Omega, log);

    vector<Servant*> all;
    all.push_back(first);
    all.push_back(second);

    log = new Logger;

    gs = new GameState(all, 10, 10, log);
}

void MainWindow::restartGameState(vector<string> team1, vector<string> team2,
                                  Team t1, Team t2, int ascensionLvl)
{
    //
}
