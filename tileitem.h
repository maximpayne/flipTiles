#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>

class TileItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scaleX READ scaleX WRITE setScaleX)
public:
    TileItem(const QPixmap &front, const QPixmap &back, QGraphicsItem *parent = nullptr);

    qreal scaleX() const { return m_scaleX; }
    void setScaleX(qreal s);

    void startFlip();

private:
    QPixmap m_front;
    QPixmap m_back;
    qreal m_scaleX;
    QSequentialAnimationGroup *group;
};

#endif // TILEITEM_H

