#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap) :
    wingPosition(WingPosition::Up),
    wingDiretion(0)
{
    setPixmap(pixmap);

    // set wings update
    QTimer * birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout, [=](){
        updatePixmap();
    });

    birdWingsTimer->start(80);

    // set y animation
    groundPosition = scenePos().y() + 290;

    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

//    yAnimation->start();

    // set rotation animation
    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
//    rotateTo(90,1200,QEasingCurve::InQuad);


}

void BirdItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle){
        if(wingDiretion){
            //up
            setPixmap(QPixmap("://images/bird_up.png").scaled(40,30));


            wingPosition = WingPosition::Up;
            wingDiretion = 0;

        }else{
            //down
            setPixmap(QPixmap("://images/bird_down.png").scaled(40,30));


            wingPosition = WingPosition::Down;
            wingDiretion = 1;
        }
    }else{
        setPixmap(QPixmap("://images/bird_middle.png").scaled(40,30));
        wingPosition = WingPosition::Middle;
    }
}

qreal BirdItem::rotation() const
{
    return m_rotation;
}

void BirdItem::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    // update rotation
    QPointF c = boundingRect().center();
    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(newRotation);
    t.translate(-c.x(),-c.y());
    setTransform(t);
}

qreal BirdItem::y() const
{

    return m_y;
}

void BirdItem::setY(qreal newY)
{
    // mover bird down
    moveBy(0, newY-m_y);
    m_y = newY;
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void BirdItem::fallToGroundIfNecessary()
{

    // restart the fall down
    if(y() < groundPosition){
        rotationAnimation->stop();


        yAnimation = new QPropertyAnimation(this, "y", this);
        yAnimation->setStartValue(scenePos().y());
        yAnimation->setEndValue(groundPosition);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(1000);
        yAnimation->start();

        rotateTo(90,1200,QEasingCurve::InQuad);

    }
}

void BirdItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();

    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/8);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);

    connect(yAnimation, &QPropertyAnimation::finished, [=](){
        fallToGroundIfNecessary();
    });

    yAnimation->start();

    rotateTo(-20,200,QEasingCurve::OutCubic);

}

void BirdItem::startFlying()
{
    yAnimation->start();
    rotateTo(90,1200,QEasingCurve::InQuad);
}

void BirdItem::freezeInPlace()
{
    yAnimation->stop();
    rotationAnimation->stop();
}
