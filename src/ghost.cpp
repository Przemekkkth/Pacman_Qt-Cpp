#include "ghost.h"

Ghost::Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY)
    : Entity(tilePosX, tilePosY), m_scattering(true), m_outOfCage(false),
      m_decision(true), m_frightened(false)
{
    setDestination(destinationX, destinationY);
    setDirection(Resources::Direction::Unset);
    //m_frightenedPixmap = m_thingsPixmap.copy()
}

void Ghost::setDestination(int x, int y)
{
    m_destinationTileX = x;
    m_destinationTileY = y;
}

int Ghost::getDestX()
{
    return m_destinationTileX;
}

int Ghost::getDestY()
{
    return m_destinationTileY;
}

void Ghost::setDirection(Resources::Direction dir)
{
    m_moving = dir;
}

Resources::Direction Ghost::getDirection()
{
    return m_moving;
}

void Ghost::move()
{
    switch (m_moving)
    {
    case Resources::Direction::Up:
        Entity::move(0, -0.2f);
        break;
    case Resources::Direction::Down:
        Entity::move(0, 0.2f);
        break;
    case Resources::Direction::Left:
        Entity::move(-0.2f, 0);
        break;
    case Resources::Direction::Right:
        Entity::move(0.2f, 0);
        break;
    }
}

bool Ghost::isScattering()
{
    return m_scattering;
}

void Ghost::setScattering(bool s)
{
    m_scattering = s;
}

bool Ghost::isOutOfCage()
{
    return m_outOfCage;
}

bool Ghost::shouldTakeDecision()
{
    return m_decision;
}

void Ghost::setTakeDecision(bool d)
{
    m_decision = d;
}

void Ghost::teleport(int x, int y)
{
    Entity::teleport(x, y);
    m_outOfCage = true;
}

void Ghost::setFrightened(bool f)
{
    if (f)
        m_frightened = 2000;
    else
        m_frightened = 0;
}

bool Ghost::isFrightened()
{
    if (m_frightened > 0) m_frightened--;
    return m_frightened > 0;
}
