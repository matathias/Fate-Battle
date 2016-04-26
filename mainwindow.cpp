#pragma once

#include "mainwindow.h"
#include "view.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QSplitter>

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

    populateScene();

    h1Splitter = new QSplitter;
    h2Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    View *view = new View("Top left view");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    view = new View("Top right view");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);

    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateScene()
{
    scene = new QGraphicsScene;

    QImage image(":/qt4logo.png");

    // Populate scene
    int xx = 0;
    int nitems = 0;
    for (int i = 0; i < 10000; i += 100) {
        ++xx;
        int yy = 0;
        for (int j = 0; j < 7000; j += 100) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            //QColor color(0,0,200,255);
            QGraphicsItem *item = new PlayFieldSquare(color, xx, yy);
            item->setPos(QPointF(i, j));
            scene->addItem(item);

            ++nitems;
        }
    }
}

void MainWindow::repopulateScene(int w, int l)
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
            QGraphicsItem *item = new PlayFieldSquare(color, xx, yy);
            item->setPos(QPointF(i, j));
            scene->addItem(item);

            ++nitems;
        }
    }

    h1Splitter = new QSplitter;
    h2Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    View *view = new View("Top left view");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    view = new View("Top right view");
    view->view()->setScene(scene);
    h1Splitter->addWidget(view);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);

    setWindowTitle(tr("Final Fate / Emblem of the Holy Grail"));
}
