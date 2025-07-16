#include "tileitem.h"
#include <QEasingCurve>

TileItem::TileItem(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_pixmap(pixmap)
{
    setPixmap(m_pixmap);
    // Anchor transformations at the top edge so the tile flips like a page
    setTransformOriginPoint(boundingRect().width() / 2.0, 0);

    m_rotation = new QGraphicsRotation(this);
    // Rotate around the X axis to mimic a calendar page
    m_rotation->setAxis(Qt::XAxis);
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

    const qreal startY = m_baseY;
    const qreal endY = m_baseY + boundingRect().height();
    setY(startY);
    m_rotation->setAngle(0);

    QParallelAnimationGroup *flip = new QParallelAnimationGroup(group);

    QPropertyAnimation *move = new QPropertyAnimation(this, "y");
    move->setDuration(1200);
    move->setStartValue(startY);
    move->setEndValue(endY);
    move->setEasingCurve(QEasingCurve::InQuad);

    QPropertyAnimation *rot = new QPropertyAnimation(m_rotation, "angle");
    rot->setDuration(1200);
    rot->setStartValue(0);
    rot->setEndValue(90);
    rot->setEasingCurve(QEasingCurve::InQuad);

    flip->addAnimation(move);
    flip->addAnimation(rot);

    group->addAnimation(flip);

    group->start();
}

void TileItem::onGroupFinished()
{
    m_rotation->setAngle(0);
    setY(m_baseY);
    emit flipFinished();
}

void TileItem::updatePixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    setPixmap(m_pixmap);
}


