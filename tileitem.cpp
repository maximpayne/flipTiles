#include "tileitem.h"
#include <QParallelAnimationGroup>

TileItem::TileItem(const QPixmap &front, const QPixmap &back, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_front(front), m_back(back), m_frontSide(true)
{
    setPixmap(m_front);
    setTransformOriginPoint(boundingRect().width() / 2.0, 0);

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

    int startAngle = 0;
    int midAngle = 90;
    int endAngle = 180;

    QSequentialAnimationGroup *rotGroup = new QSequentialAnimationGroup(group);

    QPropertyAnimation *first = new QPropertyAnimation(m_rotation, "angle");
    first->setDuration(300);
    first->setStartValue(startAngle);
    first->setEndValue(midAngle);

    connect(first, &QAbstractAnimation::finished, [this]() {
        m_frontSide = !m_frontSide;
        setPixmap(m_frontSide ? m_front : m_back);
    });

    QPropertyAnimation *second = new QPropertyAnimation(m_rotation, "angle");
    second->setDuration(300);
    second->setStartValue(midAngle);
    second->setEndValue(endAngle);

    rotGroup->addAnimation(first);
    rotGroup->addAnimation(second);

    const qreal dy = boundingRect().height() * 0.2;
    QPropertyAnimation *move = new QPropertyAnimation(this, "y");
    move->setDuration(600);
    move->setStartValue(y());
    move->setEndValue(y() + dy);

    QParallelAnimationGroup *flipGroup = new QParallelAnimationGroup(group);
    flipGroup->addAnimation(rotGroup);
    flipGroup->addAnimation(move);

    group->addAnimation(flipGroup);

    group->start();
}

void TileItem::onGroupFinished()
{
    m_rotation->setAngle(0);
    setY(y() - boundingRect().height() * 0.2);
    emit flipFinished();
}


