#include "mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), view(new QGraphicsView(this)), scene(new QGraphicsScene(this)), rows(8), cols(8)
{
    setFixedSize(800, 600);
    view->setFixedSize(800, 600);
    setCentralWidget(view);
    view->setScene(scene);
    view->installEventFilter(this);

    QPixmap img1(":/image1.jpg");
    QPixmap img2(":/image2.jpg");

    if (img1.isNull() || img2.isNull()) {
        // If images failed to load, just fill with gray
        img1 = QPixmap(800, 600);
        img1.fill(Qt::gray);
        img2 = img1;
    } else {
        img1 = img1.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        img2 = img2.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    int tileW = img1.width() / cols;
    int tileH = img1.height() / rows;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPixmap front = img1.copy(c * tileW, r * tileH, tileW, tileH);
            QPixmap back = img2.copy(c * tileW, r * tileH, tileW, tileH);

            TileItem *tile = new TileItem(front, back);
            tile->setPos(c * tileW, r * tileH);
            scene->addItem(tile);
            tiles.append(tile);
        }
    }

    scene->setSceneRect(0, 0, img1.width(), img1.height());
}

void MainWindow::startAnimation()
{
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


