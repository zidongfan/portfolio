#include "pillaritem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QDebug>
#include "scene.h"
#include "birditem.h"

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap("://images/pipe.png").scaled(50,400))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap("://images/pipe.png").scaled(50,400))),
    pastBird(false)
{

    // set pillars position
    topPillar->setPos(QPointF(0,0) - QPointF(topPillar->boundingRect().width()/2,
                                              topPillar->boundingRect().height() + 60));
    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2,
                                              60));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

    // random x and y pos
    yPos = QRandomGenerator::global()->bounded(100);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0,0) + QPoint(260 + xRandomizer, yPos));


    // set animations
    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(2000);
    xAnimation->start();

    // remove the item pillar after missing
    connect(xAnimation,&QPropertyAnimation::finished, [=](){
        qDebug() << "Animation Finished";
        scene()->removeItem(this);
        delete this;
    });



}

PillarItem::~PillarItem()
{
    qDebug() << "Deleting Pillar";
    delete topPillar;
    delete bottomPillar;

}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    xAnimation->stop();
}

void PillarItem::setX(qreal newX)
{
//    qDebug() <<"Pillar position : " << newX;
    m_x = newX;

    if(newX < 0 && !pastBird){
        pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        if(myScene){
            myScene->incrementScore();
        }
    }

    if(collideWithBird()){
        emit collideFail();
    }
    setPos(QPointF(0,0) + QPointF(x(), yPos));
}

bool PillarItem::collideWithBird()
{
    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach (QGraphicsItem *item, collidingItems){
        BirdItem *birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            return true;
        }

    }
    return false;
}
