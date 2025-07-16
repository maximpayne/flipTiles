#include "tileitem.h"

TileItem::TileItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_pixmap(pixmap)
{
    setPixmap(m_pixmap);
    setTransformOriginPoint(boundingRect().width() / 2.0, boundingRect().height());

    m_rotation = new QGraphicsRotation(this);
    m_rotation->setAxis(Qt::ZAxis);
    m_rotation->setAngle(0);
    QList<QGraphicsTransform*> transforms;
    transforms << m_rotation;
    setTransformations(transforms);

    group = new QSequentialAnimationGroup(this);
    connect(group, &QSequentialAnimationGroup::finished,
            this, &TileItem::onGroupFinished);
}

void TileItem::setBaseY(qreal y)
{
    m_baseY = y;
    setY(y);
}

void TileItem::startAnimation()
{
    if (group->state() == QAbstractAnimation::Running)
        return;

    group->clear();

    const qreal startY = m_baseY - boundingRect().height();
    const qreal endY = m_baseY;
    setY(startY);

    QPropertyAnimation *move = new QPropertyAnimation(this, "y");
    move->setDuration(600);
    move->setStartValue(startY);
    move->setEndValue(endY);

    QSequentialAnimationGroup *wobble = new QSequentialAnimationGroup(group);

    QPropertyAnimation *rot1 = new QPropertyAnimation(m_rotation, "angle");
    rot1->setDuration(100);
    rot1->setStartValue(0);
    rot1->setEndValue(-10);

    QPropertyAnimation *rot2 = new QPropertyAnimation(m_rotation, "angle");
    rot2->setDuration(100);
    rot2->setStartValue(-10);
    rot2->setEndValue(10);

    QPropertyAnimation *rot3 = new QPropertyAnimation(m_rotation, "angle");
    rot3->setDuration(100);
    rot3->setStartValue(10);
    rot3->setEndValue(0);

    wobble->addAnimation(rot1);
    wobble->addAnimation(rot2);
    wobble->addAnimation(rot3);

    group->addAnimation(move);
    group->addAnimation(wobble);

    group->start();
}

void TileItem::onGroupFinished()
{
    m_rotation->setAngle(0);
    setY(m_baseY);
    emit flipFinished();
}


