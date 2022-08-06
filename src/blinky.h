#ifndef BLINKY_H
#define BLINKY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPropertyAnimation>
#include "ghost.h"

class Blinky : public QObject, public QGraphicsPixmapItem, public Ghost
{
    Q_OBJECT
    Q_PROPERTY(qreal weak READ weak WRITE setWeak)
public:
    explicit Blinky();

    void startWeakMode();
    void stopWeakMode();
signals:

private slots:
    void updatePixmap();
    void setWeak(qreal val);
private:
    qreal weak() const;
    bool isWeak();

    QTimer m_timer;
    int m_currentIndex;
    QPixmap m_upPixmap, m_rightPixmap, m_downPixmap, m_leftPixmap;
    QPixmap m_weakPixmap;
    QPropertyAnimation *m_weakAnim;
    qreal m_weak;
    constexpr static int ANIM_SPEED = 125;

    void loadPixmap();
};

#endif // BLINKY_H
