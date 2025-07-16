#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QEvent>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), view(new QGraphicsView(this)), scene(new QGraphicsScene(this)), rows(8), cols(8), showingFront(true), finishedCount(0)
{
    setFixedSize(800, 600);
    view->setFixedSize(800, 600);
    setCentralWidget(view);
    view->setScene(scene);
    view->installEventFilter(this);

    frontImage = QPixmap(":/image1.jpg");
    backImage = QPixmap(":/image2.jpg");
    if (frontImage.isNull() || backImage.isNull()) {
        // If images failed to load, just fill with gray
        frontImage = QPixmap(800, 600);
        frontImage.fill(Qt::gray);
        backImage = frontImage;
    } else {
        frontImage = frontImage.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        backImage = backImage.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    int tileW = frontImage.width() / cols;
    int tileH = frontImage.height() / rows;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPixmap front = frontImage.copy(c * tileW, r * tileH, tileW, tileH);
            QPixmap back = backImage.copy(c * tileW, r * tileH, tileW, tileH);

            TileItem *tile = new TileItem(front, back);
            tile->setPos(c * tileW, r * tileH);
            scene->addItem(tile);
            tiles.append(tile);
            connect(tile, &TileItem::flipFinished, this, &MainWindow::onTileFinished);
        }
    }

    scene->setSceneRect(0, 0, frontImage.width(), frontImage.height());
    updateBackground();
}

void MainWindow::startAnimation()
{
    finishedCount = 0;
    scene->setBackgroundBrush(QBrush(showingFront ? frontImage : backImage));
    int delay = 0;
    int idx = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            TileItem *tile = tiles.at(idx++);
            QTimer::singleShot(delay, tile, [tile]() { tile->startFlip(); });
            delay += 100; // wave effect
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == view && event->type() == QEvent::MouseButtonPress) {
        startAnimation();
        return true; // consume
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onTileFinished()
{
    ++finishedCount;
    if (finishedCount == tiles.size()) {
        showingFront = !showingFront;
        updateBackground();
    }
}

void MainWindow::updateBackground()
{
    scene->setBackgroundBrush(QBrush(showingFront ? frontImage : backImage));
}


