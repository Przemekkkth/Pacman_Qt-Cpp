#ifndef PINKY_H
#define PINKY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "ghost.h"

class Pinky : public QObject, public QGraphicsPixmapItem, public Ghost
{
    Q_OBJECT
public:
    explicit Pinky();

signals:

};

#endif // PINKY_H
