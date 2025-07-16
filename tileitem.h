#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsRotation>
#include <QParallelAnimationGroup>

class TileItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit TileItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    void setBaseY(qreal y);
    void startAnimation();

signals:
    void flipFinished();

private slots:
    void onGroupFinished();

private:
    QPixmap m_pixmap;
    QGraphicsRotation *m_rotation;
    QSequentialAnimationGroup *group;
    qreal m_baseY {0};
};

#endif // TILEITEM_H

