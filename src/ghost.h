#ifndef GHOST_H
#define GHOST_H
#include "entity.h"
#include "resources.h"
#include <QPixmap>

class Ghost : public Entity
{
public:
    Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY);

    void setDestination(int x, int y);
    int getDestX();
    int getDestY();

    void setDirection(Resources::Direction dir);
    Resources::Direction getDirection();
    void move();

    bool isScattering();
    void setScattering(bool s);

    bool isOutOfCage();

    bool shouldTakeDecision();
    void setTakeDecision(bool d);

    void teleport(int x, int y);

    void setFrightened(bool f);
    bool isFrightened();

    bool isAnimated() const;
    void setAnimated(bool val);
protected:
    int m_destinationTileX;
    int m_destinationTileY;
    Resources::Direction m_moving;

    bool m_scattering;
    bool m_outOfCage;

    bool m_decision;

    int m_frightened;

    QPixmap m_frightenedPixmap;

    bool m_isAnimated;

    const int WEAK_MODE_SPEED;

    float m_speed;
};

#endif // GHOST_H
