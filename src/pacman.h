#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "resources.h"
#include "entity.h"
#include <queue>

class Pacman : public QObject, public QGraphicsPixmapItem, public Entity
{
    Q_OBJECT
public:
    explicit Pacman();

signals:

private slots:
    void updatePixmap();
private:
    void loadPixmap();
    std::queue<Resources::Direction> m_directions;
    int m_eatenDots;
    bool m_dead;

    QPixmap m_upPixmap, m_downPixmap, m_leftPixmap, m_rightPixmap;
    QPixmap m_deadPixmap;
    int m_currentIndex;
    QTimer m_timer;
    constexpr static float SPEED = 0.8f;
    constexpr static int ANIM_SPEED = 125;
public:

    void queueDirection(Resources::Direction dir);
    void move();
    std::queue<Resources::Direction> getDirections();
    void stop();

    void eatDot();
    int getDotsEaten();

    void setDead(bool d);
    bool isDead();

    // QGraphicsItem interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // PACMAN_H
