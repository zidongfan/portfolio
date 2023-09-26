#ifndef PILLARITEM_H
#define PILLARITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class PillarItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)

public:
    explicit PillarItem();
    ~PillarItem();

    qreal x() const;

    void freezeInPlace();

signals:
    void collideFail();

public slots:

   void setX(qreal newX);

private:
    // items
    QGraphicsPixmapItem *topPillar;
    QGraphicsPixmapItem *bottomPillar;
    int yPos;

    // animations
    QPropertyAnimation *xAnimation;

    // coliision
    bool collideWithBird();

    qreal m_x;

    bool pastBird;
};

#endif // PILLARITEM_H
