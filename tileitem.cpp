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
    connect(group, &QSequentialAnimationGroup::finished,
            this, &TileItem::onGroupFinished);
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

    QPropertyAnimation *wobble1 = new QPropertyAnimation(m_rotation, "angle", group);
    wobble1->setDuration(150);
    wobble1->setStartValue(endAngle);
    wobble1->setEndValue(endAngle + 10);

    QPropertyAnimation *wobble2 = new QPropertyAnimation(m_rotation, "angle", group);
    wobble2->setDuration(150);
    wobble2->setStartValue(endAngle + 10);
    wobble2->setEndValue(endAngle - 5);

    QPropertyAnimation *wobble3 = new QPropertyAnimation(m_rotation, "angle", group);
    wobble3->setDuration(150);
    wobble3->setStartValue(endAngle - 5);
    wobble3->setEndValue(endAngle);

    group->addAnimation(first);
    group->addAnimation(second);
    group->addAnimation(wobble1);
    group->addAnimation(wobble2);
    group->addAnimation(wobble3);

    group->start();
}

void TileItem::onGroupFinished()
{
    m_rotation->setAngle(m_frontSide ? 0 : 180);
    emit flipFinished();
}


