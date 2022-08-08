#include "clyde.h"

void Clyde::loadPixmap()
{
    m_upPixmap = m_thingsPixmap.copy(Resources::CLYDE_UP.x(), Resources::CLYDE_UP.y(), Resources::CLYDE_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_rightPixmap = m_thingsPixmap.copy(Resources::CLYDE_RIGHT.x(), Resources::CLYDE_RIGHT.y(), Resources::CLYDE_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_downPixmap = m_thingsPixmap.copy(Resources::CLYDE_DOWN.x(), Resources::CLYDE_DOWN.y(), Resources::CLYDE_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_leftPixmap = m_thingsPixmap.copy(Resources::CLYDE_LEFT.x(), Resources::CLYDE_LEFT.y(), Resources::CLYDE_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
}

Clyde::Clyde()
    : Ghost(15, 17, 1, 32), m_currentIndex(0), m_weak(0)
{
    m_speed = 0.8f;

    setPixmap(m_thingsPixmap.copy(Resources::CLYDE_DOWN.x(),Resources::CLYDE_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    loadPixmap();

    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &Clyde::updatePixmap);
    m_timer.start(ANIM_SPEED);

    m_weakAnim = new QPropertyAnimation(this, "weak", this);
    m_weakAnim->setStartValue(WEAK_MODE_SPEED);
    m_weakAnim->setDuration(WEAK_MODE_SPEED);
    m_weakAnim->setEndValue(0);

}

void Clyde::startWeakMode()
{
    m_weakAnim->start();
}

void Clyde::stopWeakMode()
{
    m_weakAnim->stop();
    m_weak = 0;
}

void Clyde::updatePixmap()
{
    if(getDirection() == Resources::Direction::Unset || !m_isAnimated)
    {
        setPixmap(m_thingsPixmap.copy(Resources::CLYDE_DOWN.x(),Resources::CLYDE_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        return;
    }

    if(!isWeak())
    {
        if(getDirection() == Resources::Direction::Up)
        {
            setPixmap(m_upPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        }
        else if(getDirection() == Resources::Direction::Right)
        {
            setPixmap(m_rightPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        }
        else if(getDirection() == Resources::Direction::Down)
        {
            setPixmap(m_downPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        }
        else if(getDirection() == Resources::Direction::Left)
        {
            setPixmap(m_leftPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        }
    }
    else
    {
        setPixmap(m_frightenedPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    }


    m_currentIndex += 1;
    m_currentIndex %= Resources::CLYDE_COUNT_ANIM_FRAMES;

}

void Clyde::setWeak(qreal val)
{
    m_weak = val;
}

qreal Clyde::weak() const
{
    return m_weak;
}

bool Clyde::isWeak()
{
    return m_weak > 0;
}
