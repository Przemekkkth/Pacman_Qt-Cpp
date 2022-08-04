#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "resources.h"
#include "entity.h"
#include <queue>

class Pacman : public QObject, public QGraphicsPixmapItem, public Entity
{
    Q_OBJECT
public:
    explicit Pacman();

signals:
private:
    std::queue<Resources::Direction> directions;
    int eatenDots;
    bool dead;

public:

    void queueDirection(Resources::Direction dir);
    void move();
    std::queue<Resources::Direction> getDirections();
    void stop();

    void eatDot();
    int getDotsEaten();

    void setDead(bool d);
    bool isDead();
};

#endif // PACMAN_H
