#pragma once

#include "mainwindow.h"
#include "view.h"
#include "ui_mainwindow.h"
#include "Servants/servanttest.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QListView>

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

void MainWindow::mainSetup()
{
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

    /* Servant stats Widget */
    QStandardItemModel *hpmpTable = new QStandardItemModel(1,2,this);
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
    QVBoxLayout *l2 = new QVBoxLayout;
    l2->addWidget(statsTable1);
    l2->addLayout(l1);

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
    // TODO (use radio buttons?)

    /* Event Log Widget */
    vector<string> eL = gs->getEventLog();
    QList<QString> eventLog;
    for (int i = 0; i < eL.size(); i++)
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

    // Setup the layouts
    nameIcon->addLayout(l2);
    nameIcon->addLayout(debuffLayout);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->addWidget(quitButton);
    layout2->addWidget(gameField);

    QVBoxLayout *layout4 = new QVBoxLayout;
    layout4->addWidget(nextServ);
    layout4->addWidget(evLogWid);

    QHBoxLayout *playerInformation = new QHBoxLayout;
    playerInformation->addLayout(nameIcon);
    playerInformation->addLayout(layout4);

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addLayout(layout2);
    layout3->addLayout(playerInformation);

    setLayout(layout3);


    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
}

void MainWindow::populateScene(int w, int l)
{
    scene = new QGraphicsScene;

    QImage image(":/qt4logo.png");

    // Populate scene
    int xx = 0;
    int nitems = 0;
    for (int i = 0; i < 100 * w; i += 100) {
        ++xx;
        int yy = 0;
        for (int j = 0; j < 100 * l; j += 100) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            //QColor color(0,0,200,255);
            // Determine the item's color based on whether or not a Servant is
            // there, and on what selection the player is currently making
            //     If Servant: show Servant Icon
            //     If No selection: gray
            //     If Selection for move: blue
            //     If selection for attack: red
            QGraphicsItem *item = new PlayFieldSquare(gs, color, xx, yy);
            item->setPos(QPointF(i, j));
            scene->addItem(item);

            ++nitems;
        }
    }

    mainSetup();
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
    //
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Final Fate"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}

void MainWindow::startGameState()
{
    //
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
