#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsRotation>

class TileItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    TileItem(const QPixmap &front, const QPixmap &back, QGraphicsItem *parent = nullptr);

    void startFlip();

private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_frontSide;
    QGraphicsRotation *m_rotation;
    QSequentialAnimationGroup *group;
};

#endif // TILEITEM_H

