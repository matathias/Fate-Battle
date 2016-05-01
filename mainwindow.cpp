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

    // Add a way to load alternate initial gameStates (i.e. specifying ascension
    // level and team makeup)

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
    cout << "Beginning of MainWindow::mainSetup().\n" << std::flush;
    clearLayout(layout());
    //cout << "In MainWindow::mainSetup(), after clearLayout() call.\n" << std::flush;
    delete layout();
    cout << "In MainWindow::mainSetup(), after delete layout() call.\n" << std::flush;

    // Setup the play field display
    View *gameField = new View("Game Field");
    gameField->view()->setScene(scene);

    // Setup the quit button
    quitButton = new QPushButton(tr("Quit"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

    /* Servant name and icon */
    QVBoxLayout *nameIcon = new QVBoxLayout;
    QString nI = QString::fromStdString(gs->getCurrentServantType());
    name = new QLabel(this);
    name->setText(nI);
    name->setAlignment(Qt::AlignCenter);
    QString tN = QString::fromStdString("Team: " + gs->getCurrentServantTeam());
    teamName = new QLabel(this);
    teamName->setText(tN);
    teamName->setAlignment(Qt::AlignCenter);
    icon = new QLabel(this);
    icon->setFrameStyle(QFrame::Panel);
    nI.prepend("../FateBattle/ServantIcons/");
    nI.append(".png");
    QPixmap ic;
    bool result = ic.load(nI);
    icon->setPixmap(ic);
    if (!result)
        icon->setText("Image failed to load");
    icon->setAlignment(Qt::AlignCenter);
    nameIcon->addWidget(icon);
    nameIcon->addWidget(name);
    nameIcon->addWidget(teamName);

    /* Servant stats Widget */
    /*QStandardItemModel *hpmpTable = new QStandardItemModel(1,2,this);
    hpmpTable->setHorizontalHeaderItem(0, new QStandardItem(QString("HP")));
    hpmpTable->setHorizontalHeaderItem(1, new QStandardItem(QString("MP")));
    QString hp = QString::fromStdString(
                    to_string(gs->getCurrentServant()->getCurrHP()) + "/" +
                    to_string(gs->getCurrentServant()->getMaxHP()));
    QString mp = QString::fromStdString(
                    to_string(gs->getCurrentServant()->getCurrMP()) + "/" +
                    to_string(gs->getCurrentServant()->getMaxMP()));
    QStandardItem *hpEntry = new QStandardItem(hp);
    QStandardItem *mpEntry = new QStandardItem(mp);
    hpmpTable->setItem(0,0,hpEntry);
    hpmpTable->setItem(0,1,mpEntry);

    QStandardItemModel *atkStatsTable = new QStandardItemModel(4,1,this);
    atkStatsTable->setVerticalHeaderItem(0, new QStandardItem(QString("MOV")));
    atkStatsTable->setVerticalHeaderItem(1, new QStandardItem(QString("STR")));
    atkStatsTable->setVerticalHeaderItem(2, new QStandardItem(QString("MAG")));
    atkStatsTable->setVerticalHeaderItem(3, new QStandardItem(QString("SKL")));
    QString mov = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getMov()));
    QString str = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getStr()));
    QString mag = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getMag()));
    QString skl = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getSkl()));
    QStandardItem *movEntry = new QStandardItem(mov);
    QStandardItem *strEntry = new QStandardItem(str);
    QStandardItem *magEntry = new QStandardItem(mag);
    QStandardItem *sklEntry = new QStandardItem(skl);
    atkStatsTable->setItem(0,0,movEntry);
    atkStatsTable->setItem(1,0,strEntry);
    atkStatsTable->setItem(2,0,magEntry);
    atkStatsTable->setItem(3,0,sklEntry);

    QStandardItemModel *defStatsTable = new QStandardItemModel(4,1,this);
    defStatsTable->setVerticalHeaderItem(0, new QStandardItem(QString("SPD")));
    defStatsTable->setVerticalHeaderItem(1, new QStandardItem(QString("DEF")));
    defStatsTable->setVerticalHeaderItem(2, new QStandardItem(QString("RES")));
    defStatsTable->setVerticalHeaderItem(3, new QStandardItem(QString("LUK")));
    QString spd = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getSpd()));
    QString def = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getDef()));
    QString res = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getRes()));
    QString luk = QString::fromStdString(
                                to_string(gs->getCurrentServant()->getLuk()));
    QStandardItem *spdEntry = new QStandardItem(spd);
    QStandardItem *defEntry = new QStandardItem(def);
    QStandardItem *resEntry = new QStandardItem(res);
    QStandardItem *lukEntry = new QStandardItem(luk);
    defStatsTable->setItem(0,0,spdEntry);
    defStatsTable->setItem(1,0,defEntry);
    defStatsTable->setItem(2,0,resEntry);
    defStatsTable->setItem(3,0,lukEntry);

    statsTable1 = new QTableView;
    statsTable1->setModel(hpmpTable);
    statsTable1->resizeColumnsToContents();
    statsTable2 = new QTableView;
    statsTable2->setModel(atkStatsTable);
    statsTable2->resizeColumnsToContents();
    statsTable3 = new QTableView;
    statsTable3->setModel(defStatsTable);
    statsTable3->resizeColumnsToContents();

    QHBoxLayout *l1 = new QHBoxLayout;
    l1->addWidget(statsTable2);
    l1->addWidget(statsTable3);
    QVBoxLayout *allStats = new QVBoxLayout;
    allStats->addWidget(statsTable1);
    allStats->addLayout(l1);*/

    /* Alternative stats layout */
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
    hplab->setText("HP");
    hplab->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    mplab->setText("MP");
    mplab->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    hpstat->setText(hp);
    hpstat->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mpstat->setText(mp);
    mpstat->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QLabel *movLab = new QLabel(this);
    movLab->setText(QString::fromStdString("Movement Range: " +
                                 to_string(gs->getCurrentServant()->getMov())));
    movLab->setAlignment(Qt::AlignCenter);

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

    QVBoxLayout *hpBox = new QVBoxLayout;
    hpBox->addWidget(hplab);
    hpBox->addWidget(hpstat);
    QVBoxLayout *mpBox = new QVBoxLayout;
    mpBox->addWidget(mplab);
    mpBox->addWidget(mpstat);
    QHBoxLayout *hpmpBox = new QHBoxLayout;
    hpmpBox->addLayout(hpBox);
    hpmpBox->addLayout(mpBox);

    QHBoxLayout *otherStats = new QHBoxLayout;
    otherStats->addWidget(atkStats);
    otherStats->addWidget(defStats);

    QVBoxLayout *allStats = new QVBoxLayout;
    allStats->addLayout(hpmpBox);
    allStats->addWidget(movLab);
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
        QString text = QString::fromStdString(actList[i] + " MP Cost: " +
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
    a->setText("\n---- Actions ----");
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
    vector<string> eL = gs->getEventLog();
    QList<QString> eventLog;
    for (unsigned int i = 0; i < eL.size(); i++)
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

    /* Next Servant Widget */
    nextServ = new QLabel(this);
    QString nS = QString::fromStdString("Next Player: " +
                                        gs->peekNextServant()->getName());
    nextServ->setText(nS);
    nextServ->setAlignment(Qt::AlignCenter);
    nextServTeam = new QLabel(this);
    QString nST = QString::fromStdString("Next Player's Team: " +
                                         gs->peekNextServant()->getTeamName());
    nextServTeam->setText(nST);
    nextServTeam->setAlignment(Qt::AlignCenter);

    // Setup the layouts
    nameIcon->addLayout(allStats);
    nameIcon->addLayout(debuffLayout);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(quitButton);
    layout2->addWidget(gameField);

    QVBoxLayout *rightMost = new QVBoxLayout;
    rightMost->addWidget(nextServ);
    rightMost->addWidget(nextServTeam);
    rightMost->addLayout(wholeActionList);
    rightMost->addWidget(evLogWid);

    QHBoxLayout *playerInformation = new QHBoxLayout;
    playerInformation->addLayout(nameIcon);
    playerInformation->addLayout(rightMost);

    mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(playerInformation);

    setLayout(mainLayout);

    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
    cout << "End of MainWindow::mainSetup().\n\n" << std::flush;
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
            Servant* spaceServ = gs->isSpaceServant(xx,yy);
            if (spaceServ != NULL)
            {
                imgPath = "../FateBattle/ServantIcons/" + spaceServ->getName()
                          + ".png";
            }
            QColor color(red, green, blue, 255);
            QGraphicsItem *item = new PlayFieldSquare(gs, color, xx, yy,
                                                      imgPath, this);

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
    cout << "Beginning of reColorScene\n" << std::flush;
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
            red = 255;
        }
        else if ((gs->getTurnState() == 1 || gs->getTurnState() == 5) &&
                 gs->isSpaceMove(x,y))
        {
            blue = 255;
        }
        else if (gs->isSpaceRealityMarble(x,y))
        {
            green = 0;
            blue = 0;
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
        Servant* spaceServ = gs->isSpaceServant(x,y);
        if (spaceServ != NULL)
        {
            imgPath = "../FateBattle/ServantIcons/" + spaceServ->getName()
                      + ".png";
        }
        QColor color(red, green, blue, 255);
        temp->changeColor(color);
        temp->setPath(imgPath);
    }

    cout << "In reColorScene(), just before mainSetup() call.\n" << std::flush;
    mainSetup();
    cout << "End of reColorScene().\n\n" << std::flush;
}

// This function is causing so many problems
// Commenting out the recursive part allows the program to not crash, but the
//     ui is not properly cleared.
// Leaving the recursive part in properly clears the ui, but makes the program
//     crash after a handful of actions. Maybe it's "accidentally" deleting
//     scene or something?
void MainWindow::clearLayout(QLayout *layout)
{
    cout << "Beginning of MainWindow::clearLayout().\n" << std::flush;
    QLayoutItem *item;
    /*while(layout != NULL && (item = layout->takeAt(0)))
    {
        if (item->layout() != NULL) {
            clearLayout(item->layout());
            //delete item->layout();
        }

        delete item->widget();

        delete item;
    }*/
    while (layout != NULL && (item = layout->takeAt(0)))
    {
        if (QWidget* widget = item->widget())
        {
            cout << "\tdeleting widget\n" << std::flush;
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
        {
            cout << "\t\trecursive call\n" << std::flush;
            clearLayout(childLayout);
        }
        cout << "deleting item\n" << std::flush;
        delete item;
    }

    //cout << "deleting layout\n" << std::flush;
    //delete layout;
    cout << "End of MainWindow::clearLayout().\n" << std::flush;
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

void MainWindow::but1()
{
    gs->setChosenAction(0);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but2()
{
    gs->setChosenAction(1);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but3()
{
    gs->setChosenAction(2);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but4()
{
    gs->setChosenAction(3);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but5()
{
    gs->setChosenAction(4);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but6()
{
    gs->setChosenAction(5);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but7()
{
    gs->setChosenAction(6);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but8()
{
    gs->setChosenAction(7);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
}
void MainWindow::but9()
{
    gs->setChosenAction(8);
    int result = gs->turnStateChoseAction();

    if (result == 30)
        gs->addToEventLog("Cannot choose action at this moment.");
    else if (result == 21)
        gs->addToEventLog("Invalid choice.");
    else if (result == 22)
        gs->addToEventLog("Not enough MP!"); // Pop up message box?

    mainSetup();
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
        int result = gs->endTurn();
        if (result == 10)
        {
            gs->addToEventLog("An error occured (endTurn()->turnStatePreTurn()).");
        }
        else if (result == 70)
        {
            gs->addToEventLog("An error occured (endTurn()).");
        }
        else if (result == 1000)
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Final Fate"));
            messageBox.setText(tr("Team Alpha Loses!"));
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            if (messageBox.exec() == QMessageBox::Ok)
                qApp->quit();
        }
        else if (result == 1001)
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Final Fate"));
            messageBox.setText(tr("Team Omega Loses!"));
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            if (messageBox.exec() == QMessageBox::Ok)
                qApp->quit();
        }
        else if (result == 1002)
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Final Fate"));
            messageBox.setText(tr("Boss Team Loses!"));
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            if (messageBox.exec() == QMessageBox::Ok)
                qApp->quit();
        }
        else
        {
            reColorScene();
        }
    }
}

void MainWindow::cancelAction()
{
    if (gs->getTurnState() == 2)
    {
        gs->addToEventLog("Undid Move Action.");
        gs->prevTurnState();
        reColorScene();
    }
    else if (gs->getTurnState() == 3)
    {
        gs->addToEventLog("Undid Action Choice.");
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
    Servant *first = new ServantTest(1, Alpha);
    Servant *second = new ServantTest(1, Omega);

    vector<Servant*> all;
    all.push_back(first);
    all.push_back(second);

    gs = new GameState(all, 10, 10);
}

void MainWindow::restartGameState(vector<string> team1, vector<string> team2,
                                  Team t1, Team t2, int ascensionLvl)
{
    //
}
