#include "tileitem.h"

TileItem::TileItem(const QPixmap &front, const QPixmap &back, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_front(front), m_back(back), m_frontSide(true)
{
    setPixmap(m_front);
    setTransformOriginPoint(boundingRect().width() / 2.0, boundingRect().height() / 2.0);

    m_rotation = new QGraphicsRotation(this);
    m_rotation->setAxis(Qt::XAxis);
    m_rotation->setAngle(0);
    QList<QGraphicsTransform*> transforms;
    transforms << m_rotation;
    setTransformations(transforms);

    group = new QSequentialAnimationGroup(this);
}

void TileItem::startFlip()
{
    if (group->state() == QAbstractAnimation::Running)
        return;

    group->clear();

    int startAngle = m_frontSide ? 0 : 180;
    int midAngle = startAngle + 90;
    int endAngle = startAngle + 180;

    QPropertyAnimation *first = new QPropertyAnimation(m_rotation, "angle", group);
    first->setDuration(300);
    first->setStartValue(startAngle);
    first->setEndValue(midAngle);

    connect(first, &QAbstractAnimation::finished, [this]() {
        m_frontSide = !m_frontSide;
        setPixmap(m_frontSide ? m_front : m_back);
    });

    QPropertyAnimation *second = new QPropertyAnimation(m_rotation, "angle", group);
    second->setDuration(300);
    second->setStartValue(midAngle);
    second->setEndValue(endAngle);

    connect(group, &QSequentialAnimationGroup::finished, [this, endAngle]() {
        m_rotation->setAngle(endAngle % 360);
    });

    group->addAnimation(first);
    group->addAnimation(second);

    group->start();
}


