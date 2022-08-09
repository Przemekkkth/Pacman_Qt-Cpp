#include "ghost.h"

Ghost::Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY)
    : Entity(tilePosX, tilePosY), m_scattering(true), m_outOfCage(false),
      m_decision(true), m_frightened(false), WEAK_MODE_SPEED(4000)
{
    setDestination(destinationX, destinationY);
    setDirection(Resources::Direction::Unset);
    m_frightenedPixmap = m_thingsPixmap.copy(Resources::FRIGHTENED_GHOST.x(), Resources::FRIGHTENED_GHOST.y(), Resources::THINGS_TILE_SIZE*Resources::FRIGHTENED_GHOST_COUNT_ANIM_FRAMES, Resources::THINGS_TILE_SIZE);
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
        Entity::move(0, -m_speed);
        break;
    case Resources::Direction::Down:
        Entity::move(0, m_speed);
        break;
    case Resources::Direction::Left:
        Entity::move(-m_speed, 0);
        break;
    case Resources::Direction::Right:
        Entity::move(m_speed, 0);
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


bool Ghost::isAnimated() const
{
    return m_isAnimated;
}

void Ghost::setAnimated(bool val)
{
    m_isAnimated = val;
}
