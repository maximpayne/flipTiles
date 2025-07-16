#include "tileitem.h"

TileItem::TileItem(const QPixmap &front, const QPixmap &back, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_front(front), m_back(back), m_scaleX(1.0)
{
    setPixmap(m_front);
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);

    group = new QSequentialAnimationGroup(this);
    QPropertyAnimation *first = new QPropertyAnimation(this, "scaleX");
    first->setDuration(300);
    first->setStartValue(1.0);
    first->setEndValue(0.0);
    QPropertyAnimation *second = new QPropertyAnimation(this, "scaleX");
    second->setDuration(300);
    second->setStartValue(0.0);
    second->setEndValue(1.0);

    connect(first, &QAbstractAnimation::finished, [this]() {
        setPixmap(m_back);
    });

    group->addAnimation(first);
    group->addAnimation(second);
}

void TileItem::setScaleX(qreal s)
{
    m_scaleX = s;
    QTransform t;
    t.scale(s, 1.0);
    setTransform(t);
}

void TileItem::startFlip()
{
    if (group->state() == QAbstractAnimation::Running)
        return;
    group->start();
}


