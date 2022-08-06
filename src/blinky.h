#ifndef BLINKY_H
#define BLINKY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "ghost.h"

class Blinky : public QObject, public QGraphicsPixmapItem, public Ghost
{
    Q_OBJECT
public:
    explicit Blinky();

signals:

private slots:
    void updatePixmap();

private:
    QTimer m_timer;
    int currentFrame;
};

#endif // BLINKY_H
