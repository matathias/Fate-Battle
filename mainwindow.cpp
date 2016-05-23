#pragma once

#include "mainwindow.h"
#include "view.h"
#include "ui_mainwindow.h"
#include "playfieldsquare.h"

#include "Servants/servanttest.h"
#include "Servants/servantsaberclaymore.h"
#include "Servants/servantsaberkatana.h"
#include "Servants/servantarcherbow.h"
#include "Servants/servantarcherpistols.h"
#include "Servants/servantarcherprojectiles.h"
#include "Servants/servantlancerhalberd.h"
#include "Servants/servantlancerlance.h"
#include "Servants/servantlancernaginata.h"
#include "Servants/servantriderchariot.h"
#include "Servants/servantriderpegasus.h"
#include "Servants/servantriderwyvern.h"
#include "Servants/servantcasterelemental.h"
#include "Servants/servantcasternecromancy.h"
#include "Servants/servantcastersummoner.h"
#include "Servants/servantberserkeraxe.h"
#include "Servants/servantberserkerclub.h"
#include "Servants/servantberserkerflail.h"
#include "Servants/servantassassindagger.h"
#include "Servants/servantassassinshuriken.h"
#include "Servants/servantavengersai.h"
#include "Servants/servantavengerscythe.h"
#include "Servants/servantsaberminion.h"
#include "Servants/servantarcherminion.h"
#include "Servants/servantcasterminion.h"
#include "Servants/bossdallas.h"
#include "Servants/bossgil.h"
#include "Servants/bossmika.h"
#include "Servants/bossrin.h"
#include "Servants/bossyuu.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QListView>
#include <QProgressBar>
#include <iostream>
#include <fstream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    // Instantiate the gameState with default parameters, then pass the
    // gameState to populateScene so that the playFieldSquares can have a
    // pointer to it

    //startGameState();

    setStringLists();
    clearSelections();

    loadGame();

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
    string accur = "Accuracy: " + to_string(gs->getCurrentServant()->getHitRate());
    vector<int> servEvade;
    Servant *gsServ = gs->getCurrentServant();
    servEvade = gsServ->getEvade();
    int ev = servEvade[0];
    string eva = "Evasion: " + to_string(ev);
    string cri = "Critical Rate: " + to_string(gs->getCurrentServant()->getCriticalRate());
    string allSTring = accur + "\n" + eva + "\n" + cri;
    extraStats->setText(QString::fromStdString(allSTring));
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
        QStandardItem *debEntry = new QStandardItem(QString("No Status Effects!"));
        debuffTab->setItem(0,0,debEntry);
    }
    else
    {
        debuffTab = new QStandardItemModel(numDebuffs, 15, this);
        debuffTab->setHorizontalHeaderItem(0, new QStandardItem(
                                               QString("Name")));
        debuffTab->setHorizontalHeaderItem(1, new QStandardItem(
                                               QString("Description")));
        debuffTab->setHorizontalHeaderItem(2, new QStandardItem(
                                               QString("Turns Left")));
        debuffTab->setHorizontalHeaderItem(3, new QStandardItem(
                                               QString("MAX HP")));
        debuffTab->setHorizontalHeaderItem(4, new QStandardItem(
                                               QString("HP")));
        debuffTab->setHorizontalHeaderItem(5, new QStandardItem(
                                               QString("MAX MP")));
        debuffTab->setHorizontalHeaderItem(6, new QStandardItem(
                                               QString("MP")));
        debuffTab->setHorizontalHeaderItem(7, new QStandardItem(
                                               QString("MOV")));
        debuffTab->setHorizontalHeaderItem(8, new QStandardItem(
                                               QString("STR")));
        debuffTab->setHorizontalHeaderItem(9, new QStandardItem(
                                               QString("MAG")));
        debuffTab->setHorizontalHeaderItem(10, new QStandardItem(
                                               QString("DEF")));
        debuffTab->setHorizontalHeaderItem(11, new QStandardItem(
                                               QString("RES")));
        debuffTab->setHorizontalHeaderItem(12, new QStandardItem(
                                               QString("SPD")));
        debuffTab->setHorizontalHeaderItem(13, new QStandardItem(
                                               QString("SKL")));
        debuffTab->setHorizontalHeaderItem(14, new QStandardItem(
                                               QString("LUK")));
        for (int i = 0; i < numDebuffs; i++)
        {
            QString debName = QString::fromStdString(deb[i]->getDebuffName());
            QString debDes = QString::fromStdString(deb[i]->getDebuffDescrip());
            QStandardItem *dN = new QStandardItem(debName);
            QStandardItem *dD = new QStandardItem(debDes);
            debuffTab->setItem(i,0,dN);
            debuffTab->setItem(i,1,dD);
            if (deb[i]->getTurnsRemaining() > 0 && deb[i]->getDebuffName().compare("Doom") != 0)
                debuffTab->setItem(i,2,new QStandardItem(QString::fromStdString(to_string(deb[i]->getTurnsRemaining()))));
            else if (deb[i]->getTurnsRemaining() > 0 && deb[i]->getDebuffName().compare("Doom") == 0)
                debuffTab->setItem(i,2,new QStandardItem(QString::fromStdString(to_string(deb[i]->getTurnsRemaining() - 1))));
            else
                debuffTab->setItem(i,2,new QStandardItem(QString::fromStdString("---")));
            debuffTab->setItem(i,3,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(MAXHP)))));
            debuffTab->setItem(i,4,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(HP)))));
            debuffTab->setItem(i,5,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(MAXMP)))));
            debuffTab->setItem(i,6,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(MP)))));
            debuffTab->setItem(i,7,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(MOV)))));
            debuffTab->setItem(i,8,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(STR)))));
            debuffTab->setItem(i,9,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(MAG)))));
            debuffTab->setItem(i,10,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(DEF)))));
            debuffTab->setItem(i,11,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(RES)))));
            debuffTab->setItem(i,12,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(SPD)))));
            debuffTab->setItem(i,13,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(SKL)))));
            debuffTab->setItem(i,14,new QStandardItem(QString::fromStdString(to_string(deb[i]->getDebuffAmount(LUK)))));
        }
    }

    debuffTable = new QTableView;
    debuffTable->setModel(debuffTab);
    debuffTable->setWordWrap(true);
    debuffTable->resizeColumnsToContents();
    debuffTable->setColumnWidth(0, 100);
    debuffTable->setColumnWidth(1, 100);
    debuffTable->resizeRowsToContents();

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
            case 9:
                button10 = new QRadioButton(text, this);
                connect(button10, SIGNAL(clicked()), this, SLOT(but10()));
                actionList->addWidget(button10);
                break;
            case 10:
                button11 = new QRadioButton(text, this);
                connect(button11, SIGNAL(clicked()), this, SLOT(but11()));
                actionList->addWidget(button11);
                break;
            case 11:
                button12 = new QRadioButton(text, this);
                connect(button12, SIGNAL(clicked()), this, SLOT(but12()));
                actionList->addWidget(button12);
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

    /* Servant HP Widget */
    vector<Servant*> teamOneHP = gs->getAlphaTeam();
    vector<Servant*> teamTwoHP;
    string tOneHP = "";
    string tTwoHP = "";
    if (teamOneHP.size() == 0)
    {
        tOneHP = "Omega Team HP\n\n";
        teamOneHP = gs->getOmegaTeam();
        tTwoHP = "Boss Team HP\n\n";
        teamTwoHP = gs->getBossTeam();
    }
    else
    {
        tOneHP = "Alpha Team HP\n\n";
        teamTwoHP = gs->getOmegaTeam();
        tTwoHP = "Omega Team HP\n\n";
        if (teamTwoHP.size() == 0)
        {
            tTwoHP = "Boss Team HP\n\n";
            teamTwoHP = gs->getBossTeam();
        }
    }

    for (unsigned int i = 0; i < teamOneHP.size(); i++)
    {
        tOneHP += teamOneHP[i]->getFullName() + ":  " +
                to_string(teamOneHP[i]->getCurrHP()) + " / " +
                to_string(teamOneHP[i]->getMaxHP()) + "\n\n";
    }
    for (unsigned int i = 0; i < teamTwoHP.size(); i++)
    {
        tTwoHP += teamTwoHP[i]->getFullName() + ":  " +
                to_string(teamTwoHP[i]->getCurrHP()) + " / " +
                to_string(teamTwoHP[i]->getMaxHP()) + "\n\n";
    }

    QLabel *teamOneHPLabel = new QLabel;
    QLabel *teamTwoHPLabel = new QLabel;
    teamOneHPLabel->setText(QString::fromStdString(tOneHP));
    teamTwoHPLabel->setText(QString::fromStdString(tTwoHP));
    teamOneHPLabel->setAlignment(Qt::AlignCenter);
    teamTwoHPLabel->setAlignment(Qt::AlignCenter);
    teamOneHPLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    teamTwoHPLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);

    // Setup the layouts
    nameIcon->addLayout(allStats);
    //nameIcon->addLayout(debuffLayout);

    QVBoxLayout *allHPLayout = new QVBoxLayout;
    allHPLayout->addWidget(nextServ);
    allHPLayout->addWidget(teamOneHPLabel);
    allHPLayout->addWidget(teamTwoHPLabel);
    allHPLayout->addWidget(quitButton);
    allHPLayout->setStretch(0, 1);
    allHPLayout->setStretch(1, 10);
    allHPLayout->setStretch(2, 10);

    QVBoxLayout *layout2 = new QVBoxLayout;
    //layout2->addWidget(quitButton);
    layout2->addWidget(gameField);
    //layout2->addWidget(errorLabel);
    //layout2->addWidget(errorLabel);

    QVBoxLayout *rightMost = new QVBoxLayout;
    //rightMost->addWidget(nextServ);
    rightMost->addLayout(wholeActionList);
    rightMost->addWidget(errorLabel);
    rightMost->addWidget(evLogLabel);
    rightMost->addWidget(evLogWid);

    QHBoxLayout *playerInformation = new QHBoxLayout;
    playerInformation->addLayout(nameIcon);
    playerInformation->addLayout(rightMost);

    QVBoxLayout *allInformation = new QVBoxLayout;
    allInformation->addLayout(playerInformation);
    allInformation->addLayout(debuffLayout);

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(allHPLayout);
    mainLayout->addLayout(layout2);
    //mainLayout->addLayout(playerInformation);
    mainLayout->addLayout(allInformation);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 5);
    mainLayout->setStretch(2, 4);

    setLayout(mainLayout);

    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
}

void MainWindow::populateScene(int w, int l)
{
    // This part MAY cause problems...
    //scene->clear();
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
            if (gs->getCurrentServant()->getCurrLoc().x == xx &&
                    gs->getCurrentServant()->getCurrLoc().y == yy)
            {
                blue = 0;
                green = 255;
                red = 0;
            }
            else if (gs->getTurnState() == 3 && gs->isSpaceSelection(xx,yy))
            {
                blue = 0;
                green = 0;
                red = 255;
            }
            else if ((gs->getTurnState() == 1 || gs->getTurnState() == 5) &&
                     gs->isSpaceMove(xx,yy))
            {
                blue = 255;
            }
            else if (gs->isSpaceRealityMarble(xx,yy))
            {
                green = 50;
                blue = 50;
                red = 50;
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
                    red = 100;
                    green = 100;
                    blue = 100;
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
        if (gs->getCurrentServant()->getCurrLoc().x == x &&
                gs->getCurrentServant()->getCurrLoc().y == y)
        {
            blue = 0;
            green = 255;
            red = 0;
        }
        else if (gs->getTurnState() == 3 && gs->isSpaceSelection(x,y))
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
                red = 100;
                green = 100;
                blue = 100;
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

void MainWindow::quitGame()
{
    printErrorLog();
    printEventLog();

    QMessageBox messageBox;
    messageBox.setWindowTitle(QObject::tr("Final Fate"));
    messageBox.setText(QObject::tr("Quitting Game."));
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);
    messageBox.exec();

    qApp->quit();
}

void MainWindow::printErrorLog()
{
    vector<string> errLog = log->getErrorLog();
    std::ofstream outFile;
    outFile.open("ErrorLog.txt");

    for (unsigned int i = 0; i < errLog.size(); i++)
    {
        outFile << errLog[i] << endl;
    }

    outFile.close();
}

void MainWindow::printEventLog()
{
    vector<string> eventLog = log->getEventLog();
    std::ofstream outFile;
    outFile.open("EventLog.txt");

    for (unsigned int i = 0; i < eventLog.size(); i++)
    {
        outFile << eventLog[i] << endl;
    }

    outFile.close();
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
        // The all servants on the first team MUST be listed AFTER the team name
        //  but BEFORE the team name of the second team.
        int ascLvl = -1;
        int fieldWidth = -1;
        int fieldLength = -1;
        Team tOne = All;
        Team tTwo = All;
        vector<string> tOneNames;
        vector<string> tTwoNames;
        QString line;
        do
        {
            line = in.readLine();
            string input = line.toStdString();
            if(input.compare("Ascension 1") == 0)
                ascLvl = 0;
            else if (input.compare("Ascension 2") == 0)
                ascLvl = 1;
            else if (input.compare("Ascension 3") == 0)
                ascLvl = 2;
            else if (input.substr(0,13).compare("Field Width: ") == 0)
                fieldWidth = stoi(input.substr(13));
            else if (input.substr(0,14).compare("Field Length: ") == 0)
                fieldLength = stoi(input.substr(14));
            else if (input.compare("Alpha") == 0 && tOne == All)
                tOne = Alpha;
            else if (input.compare("Omega") == 0 && tOne == All)
                tOne = Omega;
            else if (input.compare("Boss") == 0 && tOne == All)
                tOne = Boss;
            else if (input.compare("Alpha") == 0)
                tTwo = Alpha;
            else if (input.compare("Omega") == 0)
                tTwo = Omega;
            else if (input.compare("Boss") == 0)
                tTwo = Boss;
            // Parse servant names
            else if (tOne != All && tTwo == All)
            {
                // First team's servants
                tOneNames.push_back(input);
            }
            else
            {
                // Second team's servants
                tTwoNames.push_back(input);
            }
        } while (!line.isNull());

        file.close();

        // If the file did not give enough data to start a new game
        if (ascLvl == -1 || tOne == All || tTwo == All || tOneNames.size() == 0
                || tTwoNames.size() == 0 || fieldWidth == -1 || fieldLength == -1)
        {
            QMessageBox::critical(this, tr("Error"), tr("File did not give enough information to start new game"));
            return;
        }
        else
        {
            restartGameState(tOneNames, tTwoNames, tOne, tTwo, ascLvl,
                             fieldWidth, fieldLength);
        }
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
        quitGame();
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
void MainWindow::but10()
{
    gs->setChosenAction(9);
    buttonProcess();
}
void MainWindow::but11()
{
    gs->setChosenAction(10);
    buttonProcess();
}
void MainWindow::but12()
{
    gs->setChosenAction(11);
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

// Load Dialog Comboboxes
void MainWindow::loadCBTeam1_0(int index)
{
    teamOne[0] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_1(int index)
{
    teamOne[1] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_2(int index)
{
    teamOne[2] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_3(int index)
{
    teamOne[3] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_4(int index)
{
    teamOne[4] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_5(int index)
{
    teamOne[5] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_6(int index)
{
    teamOne[6] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam1_7(int index)
{
    teamOne[7] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_0(int index)
{
    teamTwo[0] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_1(int index)
{
    teamTwo[1] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_2(int index)
{
    teamTwo[2] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_3(int index)
{
    teamTwo[3] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_4(int index)
{
    teamTwo[4] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_5(int index)
{
    teamTwo[5] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_6(int index)
{
    teamTwo[6] = servantNames.at(index).toStdString();
}
void MainWindow::loadCBTeam2_7(int index)
{
    teamTwo[7] = servantNames.at(index).toStdString();
}

void MainWindow::loadCBTeam1(int index)
{
    string cbTeam = teamNames.at(index).toStdString();
    if (cbTeam.compare("Alpha") == 0)
        tOne = Alpha;
    else if (cbTeam.compare("Omega") == 0)
        tOne = Omega;
    else
        tOne = Boss;
}
void MainWindow::loadCBTeam2(int index)
{
    string cbTeam = teamNames.at(index).toStdString();
    if (cbTeam.compare("Alpha") == 0)
        tTwo = Alpha;
    else if (cbTeam.compare("Omega") == 0)
        tTwo = Omega;
    else
        tTwo = Boss;
}

void MainWindow::loadAscLvl(int index)
{
    ascLevel = index;
}

void MainWindow::loadFieldLength(int index)
{
    fieldLen = index;
}

void MainWindow::loadFieldWidth(int index)
{
    fieldWid = index;
}


/********** END OF SLOTS **********/

void MainWindow::redrawEverything()
{
    populateScene(gs->getFieldWidth(), gs->getFieldLength());
}

int MainWindow::capZero(int num)
{
    if (num < 0)
        return 0;
    else
        return num;
}

void MainWindow::loadGame()
{
    QDialog *initDialog = new QDialog;
    QComboBox *team1Box0 = new QComboBox;
    QComboBox *team1Box1 = new QComboBox;
    QComboBox *team1Box2 = new QComboBox;
    QComboBox *team1Box3 = new QComboBox;
    QComboBox *team1Box4 = new QComboBox;
    QComboBox *team1Box5 = new QComboBox;
    QComboBox *team1Box6 = new QComboBox;
    QComboBox *team1Box7 = new QComboBox;
    QComboBox *team2Box0 = new QComboBox;
    QComboBox *team2Box1 = new QComboBox;
    QComboBox *team2Box2 = new QComboBox;
    QComboBox *team2Box3 = new QComboBox;
    QComboBox *team2Box4 = new QComboBox;
    QComboBox *team2Box5 = new QComboBox;
    QComboBox *team2Box6 = new QComboBox;
    QComboBox *team2Box7 = new QComboBox;

    QComboBox *team1Box = new QComboBox;
    QComboBox *team2Box = new QComboBox;

    QComboBox *ascLevelBox = new QComboBox;

    QSpinBox *fieldLengthBox = new QSpinBox;
    QSpinBox *fieldWidthBox = new QSpinBox;

    QPushButton *startGameButton = new QPushButton("Start Game");
    QPushButton *quitGameButton = new QPushButton("Quit");

    team1Box0->addItems(servantNames);
    team1Box1->addItems(servantNames);
    team1Box2->addItems(servantNames);
    team1Box3->addItems(servantNames);
    team1Box4->addItems(servantNames);
    team1Box5->addItems(servantNames);
    team1Box6->addItems(servantNames);
    team1Box7->addItems(servantNames);
    team2Box0->addItems(servantNames);
    team2Box1->addItems(servantNames);
    team2Box2->addItems(servantNames);
    team2Box3->addItems(servantNames);
    team2Box4->addItems(servantNames);
    team2Box5->addItems(servantNames);
    team2Box6->addItems(servantNames);
    team2Box7->addItems(servantNames);

    team1Box->addItems(teamNames);
    team2Box->addItems(teamNames);

    ascLevelBox->addItems(ascLevelNames);

    fieldLengthBox->setMinimum(10);
    fieldLengthBox->setMaximum(100);
    fieldLengthBox->setWrapping(true);
    fieldWidthBox->setMinimum(10);
    fieldWidthBox->setMaximum(100);
    fieldWidthBox->setWrapping(true);

    connect(team1Box0, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_0(int)));
    connect(team1Box1, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_1(int)));
    connect(team1Box2, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_2(int)));
    connect(team1Box3, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_3(int)));
    connect(team1Box4, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_4(int)));
    connect(team1Box5, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_5(int)));
    connect(team1Box6, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_6(int)));
    connect(team1Box7, SIGNAL(activated(int)), this, SLOT(loadCBTeam1_7(int)));
    connect(team2Box0, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_0(int)));
    connect(team2Box1, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_1(int)));
    connect(team2Box2, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_2(int)));
    connect(team2Box3, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_3(int)));
    connect(team2Box4, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_4(int)));
    connect(team2Box5, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_5(int)));
    connect(team2Box6, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_6(int)));
    connect(team2Box7, SIGNAL(activated(int)), this, SLOT(loadCBTeam2_7(int)));

    connect(team1Box, SIGNAL(activated(int)), this, SLOT(loadCBTeam1(int)));
    connect(team2Box, SIGNAL(activated(int)), this, SLOT(loadCBTeam2(int)));

    connect(ascLevelBox, SIGNAL(activated(int)), this, SLOT(loadAscLvl(int)));

    connect(fieldLengthBox, SIGNAL(valueChanged(int)), this, SLOT(loadFieldLength(int)));
    connect(fieldWidthBox, SIGNAL(valueChanged(int)), this, SLOT(loadFieldWidth(int)));

    connect(startGameButton, SIGNAL(clicked()), initDialog, SLOT(accept()));
    connect(quitGameButton, SIGNAL(clicked()), initDialog, SLOT(reject()));

    // Create all the labels
    QLabel *team1Label = new QLabel("Team One");
    QLabel *team2Label = new QLabel("Team Two");
    QLabel *team1ChooseLabel = new QLabel("Choose Team: ");
    QLabel *team2ChooseLabel = new QLabel("Choose Team: ");
    QLabel *team1ServantLabel = new QLabel("Team Composition");
    QLabel *team2ServantLabel = new QLabel("Team Composition");
    QLabel *fieldLenLabel = new QLabel("Field Length");
    QLabel *fieldWidLabel = new QLabel("Field Width");
    QLabel *ascLvlLabel = new QLabel("Ascension Level");
    team1Label->setAlignment(Qt::AlignCenter);
    team2Label->setAlignment(Qt::AlignCenter);
    team1ServantLabel->setAlignment(Qt::AlignCenter);
    team2ServantLabel->setAlignment(Qt::AlignCenter);
    fieldLenLabel->setAlignment(Qt::AlignCenter);
    fieldWidLabel->setAlignment(Qt::AlignCenter);
    ascLvlLabel->setAlignment(Qt::AlignCenter);

    /** Set up the layouts **/
    // Team 1 Layout
    QVBoxLayout *team1ServantBoxes = new QVBoxLayout;
    team1ServantBoxes->addWidget(team1ServantLabel);
    team1ServantBoxes->addWidget(team1Box0);
    team1ServantBoxes->addWidget(team1Box1);
    team1ServantBoxes->addWidget(team1Box2);
    team1ServantBoxes->addWidget(team1Box3);
    team1ServantBoxes->addWidget(team1Box4);
    team1ServantBoxes->addWidget(team1Box5);
    team1ServantBoxes->addWidget(team1Box6);
    team1ServantBoxes->addWidget(team1Box7);
    team1ServantBoxes->setSpacing(20);
    QHBoxLayout *team1TeamBoxLay = new QHBoxLayout;
    team1TeamBoxLay->addWidget(team1ChooseLabel);
    team1TeamBoxLay->addWidget(team1Box);
    QVBoxLayout *team1AllLayout = new QVBoxLayout;
    team1AllLayout->addWidget(team1Label);
    team1AllLayout->addLayout(team1TeamBoxLay);
    team1AllLayout->addLayout(team1ServantBoxes);

    // Team 2 Layout
    QVBoxLayout *team2ServantBoxes = new QVBoxLayout;
    team2ServantBoxes->addWidget(team2ServantLabel);
    team2ServantBoxes->addWidget(team2Box0);
    team2ServantBoxes->addWidget(team2Box1);
    team2ServantBoxes->addWidget(team2Box2);
    team2ServantBoxes->addWidget(team2Box3);
    team2ServantBoxes->addWidget(team2Box4);
    team2ServantBoxes->addWidget(team2Box5);
    team2ServantBoxes->addWidget(team2Box6);
    team2ServantBoxes->addWidget(team2Box7);
    team2ServantBoxes->setSpacing(20);
    QHBoxLayout *team2TeamBoxLay = new QHBoxLayout;
    team2TeamBoxLay->addWidget(team2ChooseLabel);
    team2TeamBoxLay->addWidget(team2Box);
    QVBoxLayout *team2AllLayout = new QVBoxLayout;
    team2AllLayout->addWidget(team2Label);
    team2AllLayout->addLayout(team2TeamBoxLay);
    team2AllLayout->addLayout(team2ServantBoxes);

    // Field Layout
    QVBoxLayout *fieldLayout = new QVBoxLayout;
    fieldLayout->addWidget(fieldLenLabel);
    fieldLayout->addWidget(fieldLengthBox);
    //fieldLayout->addSpacing(3);
    fieldLayout->addWidget(fieldWidLabel);
    fieldLayout->addWidget(fieldWidthBox);
    fieldLayout->addWidget(ascLvlLabel);
    fieldLayout->addWidget(ascLevelBox);

    // Main Layout
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(team1AllLayout);
    mainLayout->addLayout(fieldLayout);
    mainLayout->addLayout(team2AllLayout);
    mainLayout->setSpacing(10);

    // Everything Layout
    QVBoxLayout *everythingLayout = new QVBoxLayout;
    everythingLayout->addLayout(mainLayout);
    everythingLayout->addWidget(startGameButton);
    everythingLayout->addWidget(quitGameButton);

    initDialog->setLayout(everythingLayout);
    initDialog->setWindowTitle("Final Fate Game Setup");

    int result = initDialog->exec();

    if (result == QDialog::Rejected)
    {
        // Quit
        qApp->quit();
        exit(0);
    }

    // Check that the inputs are valid. If they aren't, tell the user and bring
    //  up the dialog box again.
    bool invalidInputs = false;
    string errString = "Invalid Input(s):\n";
    if (tOne == tTwo)
    {
        errString += "Selected teams are the same.\n";
        invalidInputs = true;
    }
    if (isTeamEmpty(teamOne))
    {
        errString += "Team One has no selected Servants.\n";
        invalidInputs = true;
    }
    if (isTeamEmpty(teamTwo))
    {
        errString += "Team Two has no selected Servants.\n";
        invalidInputs = true;
    }
    if (ascLevel < 0 || ascLevel > 2)
    {
        errString += "Invalid Ascension Level.\n";
        invalidInputs = true;
    }

    if (invalidInputs)
    {
        QMessageBox checkMessage;
        checkMessage.setWindowTitle(QObject::tr("Final Fate"));
        checkMessage.setText(QString::fromStdString(errString));
        checkMessage.setStandardButtons(QMessageBox::Ok);
        checkMessage.setDefaultButton(QMessageBox::Ok);
        checkMessage.exec();

        // Clear the selections
        clearSelections();

        // get the dialog again
        loadGame();
    }

    initGameState();
}

void MainWindow::clearSelections()
{
    teamOne.clear();
    teamTwo.clear();
    tOne = Alpha;
    tTwo = Alpha;
    ascLevel = 0;
    fieldWid = 10;
    fieldLen = 10;

    while (teamOne.size() < 8)
    {
        teamOne.push_back("No Servant");
        teamTwo.push_back("No Servant");
    }
}

void MainWindow::setStringLists()
{
    servantNames.clear();
    teamNames.clear();
    ascLevelNames.clear();

    // Servant Names
    servantNames << "No Servant";
    servantNames << "Saber - Claymore" << "Saber - Katana";
    servantNames << "Lancer - Lance" << "Lancer - Halberd" << "Lancer - Naginata";
    servantNames << "Archer - Bow" << "Archer - Pistols" << "Archer - Projectiles";
    servantNames << "Rider - Pegasus" << "Rider - Wyvern" << "Rider - Chariot";
    servantNames << "Caster - Elemental" << "Caster - Necromancer" << "Caster - Summoner";
    servantNames << "Berserker - Axe" << "Berserker - Club" << "Berserker - Flail";
    servantNames << "Assassin - Daggers" << "Assassin - Shuriken";
    servantNames << "Avenger - Sai" << "Avenger - Scythe";
    servantNames << "Boss: Dallas Jones" << "Boss: Rin Tohsaka";
    servantNames << "Boss: Mika" << "Boss: Yuu";
    servantNames << "Boss: Gilgamesh";
    servantNames << "Minion: Saber" << "Minion: Archer" << "Minion: Caster";

    // Team Names
    teamNames << "Alpha" << "Omega" << "Boss";

    // Ascension Levels
    ascLevelNames << "Base Level" << "1st Ascension" << "Final Ascension";
}

bool MainWindow::isTeamEmpty(vector<string> team)
{
    bool isEmpty = true;

    for (unsigned int i = 0; i < team.size() && isEmpty; i++)
    {
        // Start at 1 to skip "No Servant"
        for (int j = 1; j < servantNames.length(); j++)
        {
            if (team[i].compare(servantNames.at(j).toStdString()) == 0)
                isEmpty = false;
        }
    }

    return isEmpty;
}

void MainWindow::startGameState()
{
    log = new Logger;

    Servant *first = new ServantSaberClaymore(1, Alpha, log);
    Servant *second = new ServantSaberKatana(1, Omega, log);

    vector<Servant*> all;
    all.push_back(first);
    all.push_back(second);

    gs = new GameState(all, 10, 10, log);
}

void MainWindow::initGameState()
{
    log = new Logger;
    vector<Servant*> all;
    for (unsigned int i = 0; i < teamOne.size(); i++)
    {
        string servName = teamOne[i];
        Team te = tOne;
        if (servName.compare("Saber - Claymore") == 0)
            all.push_back(new ServantSaberClaymore(ascLevel, te, log));
        else if (servName.compare("Saber - Katana") == 0)
            all.push_back(new ServantSaberKatana(ascLevel, te, log));
        else if (servName.compare("Lancer - Lance") == 0)
            all.push_back(new ServantLancerLance(ascLevel, te, log));
        else if (servName.compare("Lancer - Halberd") == 0)
            all.push_back(new ServantLancerHalberd(ascLevel, te, log));
        else if (servName.compare("Lancer - Naginata") == 0)
            all.push_back(new ServantLancerNaginata(ascLevel, te, log));
        else if (servName.compare("Archer - Bow") == 0)
            all.push_back(new ServantArcherBow(ascLevel, te, log));
        else if (servName.compare("Archer - Pistols") == 0)
            all.push_back(new ServantArcherPistols(ascLevel, te, log));
        else if (servName.compare("Archer - Projectiles") == 0)
            all.push_back(new ServantArcherProjectiles(ascLevel, te, log));
        else if (servName.compare("Rider - Pegasus") == 0)
            all.push_back(new ServantRiderPegasus(ascLevel, te, log));
        else if (servName.compare("Rider - Wyvern") == 0)
            all.push_back(new ServantRiderWyvern(ascLevel, te, log));
        else if (servName.compare("Rider - Chariot") == 0)
            all.push_back(new ServantRiderChariot(ascLevel, te, log));
        else if (servName.compare("Caster - Elemental") == 0)
            all.push_back(new ServantCasterElemental(ascLevel, te, log));
        else if (servName.compare("Caster - Necromancer") == 0)
            all.push_back(new ServantCasterNecromancy(ascLevel, te, log));
        else if (servName.compare("Caster - Summoner") == 0)
            all.push_back(new ServantCasterSummoner(ascLevel, te, log));
        else if (servName.compare("Berserker - Axe") == 0)
            all.push_back(new ServantBerserkerAxe(ascLevel, te, log));
        else if (servName.compare("Berserker - Club") == 0)
            all.push_back(new ServantBerserkerClub(ascLevel, te, log));
        else if (servName.compare("Berserker - Flail") == 0)
            all.push_back(new ServantBerserkerFlail(ascLevel, te, log));
        else if (servName.compare("Assassin - Daggers") == 0)
            all.push_back(new ServantAssassinDagger(ascLevel, te, log));
        else if (servName.compare("Assassin - Shuriken") == 0)
            all.push_back(new ServantAssassinShuriken(ascLevel, te, log));
        else if (servName.compare("Avenger - Sai") == 0)
            all.push_back(new ServantAvengerSai(ascLevel, te, log));
        else if (servName.compare("Avenger - Scythe") == 0)
            all.push_back(new ServantAvengerScythe(ascLevel, te, log));
        else if (servName.compare("Minion: Saber") == 0)
            all.push_back(new ServantSaberMinion(ascLevel, te, log));
        else if (servName.compare("Minion: Archer") == 0)
            all.push_back(new ServantArcherMinion(ascLevel, te, log));
        else if (servName.compare("Minion: Caster") == 0)
            all.push_back(new ServantCasterMinion(ascLevel, te, log));
        else if (servName.compare("Boss: Mika") == 0)
            all.push_back(new BossMika(ascLevel, te, log));
        else if (servName.compare("Boss: Yuu") == 0)
            all.push_back(new BossYuu(ascLevel, te, log));
        else if (servName.compare("Boss: Rin Tohsaka") == 0)
            all.push_back(new BossRin(ascLevel, te, log));
        else if (servName.compare("Boss: Dallas Jones") == 0)
            all.push_back(new BossDallas(ascLevel, te, log));
        else if (servName.compare("Boss: Gilgamesh") == 0)
            all.push_back(new BossGil(ascLevel, te, log));
    }
    for (unsigned int i = 0; i < teamTwo.size(); i++)
    {
        string servName = teamTwo[i];
        Team te = tTwo;
        if (servName.compare("Saber - Claymore") == 0)
            all.push_back(new ServantSaberClaymore(ascLevel, te, log));
        else if (servName.compare("Saber - Katana") == 0)
            all.push_back(new ServantSaberKatana(ascLevel, te, log));
        else if (servName.compare("Lancer - Lance") == 0)
            all.push_back(new ServantLancerLance(ascLevel, te, log));
        else if (servName.compare("Lancer - Halberd") == 0)
            all.push_back(new ServantLancerHalberd(ascLevel, te, log));
        else if (servName.compare("Lancer - Naginata") == 0)
            all.push_back(new ServantLancerNaginata(ascLevel, te, log));
        else if (servName.compare("Archer - Bow") == 0)
            all.push_back(new ServantArcherBow(ascLevel, te, log));
        else if (servName.compare("Archer - Pistols") == 0)
            all.push_back(new ServantArcherPistols(ascLevel, te, log));
        else if (servName.compare("Archer - Projectiles") == 0)
            all.push_back(new ServantArcherProjectiles(ascLevel, te, log));
        else if (servName.compare("Rider - Pegasus") == 0)
            all.push_back(new ServantRiderPegasus(ascLevel, te, log));
        else if (servName.compare("Rider - Wyvern") == 0)
            all.push_back(new ServantRiderWyvern(ascLevel, te, log));
        else if (servName.compare("Rider - Chariot") == 0)
            all.push_back(new ServantRiderChariot(ascLevel, te, log));
        else if (servName.compare("Caster - Elemental") == 0)
            all.push_back(new ServantCasterElemental(ascLevel, te, log));
        else if (servName.compare("Caster - Necromancer") == 0)
            all.push_back(new ServantCasterNecromancy(ascLevel, te, log));
        else if (servName.compare("Caster - Summoner") == 0)
            all.push_back(new ServantCasterSummoner(ascLevel, te, log));
        else if (servName.compare("Berserker - Axe") == 0)
            all.push_back(new ServantBerserkerAxe(ascLevel, te, log));
        else if (servName.compare("Berserker - Club") == 0)
            all.push_back(new ServantBerserkerClub(ascLevel, te, log));
        else if (servName.compare("Berserker - Flail") == 0)
            all.push_back(new ServantBerserkerFlail(ascLevel, te, log));
        else if (servName.compare("Assassin - Daggers") == 0)
            all.push_back(new ServantAssassinDagger(ascLevel, te, log));
        else if (servName.compare("Assassin - Shuriken") == 0)
            all.push_back(new ServantAssassinShuriken(ascLevel, te, log));
        else if (servName.compare("Avenger - Sai") == 0)
            all.push_back(new ServantAvengerSai(ascLevel, te, log));
        else if (servName.compare("Avenger - Scythe") == 0)
            all.push_back(new ServantAvengerScythe(ascLevel, te, log));
        else if (servName.compare("Minion: Saber") == 0)
            all.push_back(new ServantSaberMinion(ascLevel, te, log));
        else if (servName.compare("Minion: Archer") == 0)
            all.push_back(new ServantArcherMinion(ascLevel, te, log));
        else if (servName.compare("Minion: Caster") == 0)
            all.push_back(new ServantCasterMinion(ascLevel, te, log));
        else if (servName.compare("Boss: Mika") == 0)
            all.push_back(new BossMika(ascLevel, te, log));
        else if (servName.compare("Boss: Yuu") == 0)
            all.push_back(new BossYuu(ascLevel, te, log));
        else if (servName.compare("Boss: Rin Tohsaka") == 0)
            all.push_back(new BossRin(ascLevel, te, log));
        else if (servName.compare("Boss: Dallas Jones") == 0)
            all.push_back(new BossDallas(ascLevel, te, log));
        else if (servName.compare("Boss: Gilgamesh") == 0)
            all.push_back(new BossGil(ascLevel, te, log));
    }

    gs = new GameState(all, fieldLen, fieldWid, log);
}

// Finish writing this when all classes are complete...
void MainWindow::restartGameState(vector<string> team1, vector<string> team2,
                                  Team t1, Team t2, int ascensionLvl, int fX, int fY)
{
    teamOne = team1;
    teamTwo = team2;
    tOne = t1;
    tTwo = t2;
    ascLevel = ascensionLvl;
    fieldWid = fX;
    fieldLen = fY;

    initGameState();

    redrawEverything();
}
