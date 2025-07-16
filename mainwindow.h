#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "tileitem.h"
#include <QEvent>
#include <QPixmap>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void startAnimation();
    QGraphicsView *view;
    QGraphicsScene *scene;
    QVector<TileItem*> tiles;
    int rows;
    int cols;
    QPixmap frontImage;
    QPixmap backImage;
    bool showingFront;
    int finishedCount;

    void updateBackground();
    void onTileFinished();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H

