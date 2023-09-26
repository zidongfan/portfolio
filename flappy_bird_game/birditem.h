#ifndef BIRDITEM_H
#define BIRDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class BirdItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal y READ y WRITE setY)
public:
    explicit BirdItem(QPixmap pixmap);

    qreal rotation() const;


    qreal y() const;




    void shootUp();

    void startFlying();

    void freezeInPlace();



signals:

public slots:
    void setRotation(qreal newRotation);

    void setY(qreal newY);

    void rotateTo(const qreal &end, const int& duration, const QEasingCurve& curve);

    void fallToGroundIfNecessary();

private:
    enum WingPosition{
        Up,
        Middle,
        Down
    };

    void updatePixmap();

    WingPosition wingPosition;
    bool wingDiretion; // 0 :down, 1; up
    qreal m_rotation;
    qreal m_y;

    QPropertyAnimation *rotationAnimation;
    QPropertyAnimation *yAnimation;
    qreal groundPosition;
};

#endif // BIRDITEM_H
