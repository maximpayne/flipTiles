#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "tileitem.h"

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
};

#endif // MAINWINDOW_H

