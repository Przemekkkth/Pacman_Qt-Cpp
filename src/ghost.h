#ifndef GHOST_H
#define GHOST_H
#include "entity.h"
#include "resources.h"

class Ghost : public Entity
{
private:
    int m_destinationTileX;
    int m_destinationTileY;
    Resources::Direction m_moving;

    bool m_scattering;
    bool m_outOfCage;

    bool m_decision;

    int m_frightened;
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
};

#endif // GHOST_H
