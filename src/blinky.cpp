#include "blinky.h"

void Blinky::loadPixmap()
{
    m_upPixmap = m_thingsPixmap.copy(Resources::BLINKY_UP.x(), Resources::BLINKY_UP.y(), Resources::BLINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_rightPixmap = m_thingsPixmap.copy(Resources::BLINKY_RIGHT.x(), Resources::BLINKY_RIGHT.y(), Resources::BLINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_downPixmap = m_thingsPixmap.copy(Resources::BLINKY_DOWN.x(), Resources::BLINKY_DOWN.y(), Resources::BLINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_leftPixmap = m_thingsPixmap.copy(Resources::BLINKY_LEFT.x(), Resources::BLINKY_LEFT.y(), Resources::BLINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
}

Blinky::Blinky()
    : Ghost(13, 14, 3, 4), m_weak(0)
{
    setPixmap(m_thingsPixmap.copy(Resources::BLINKY_DOWN.x(),Resources::BLINKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    loadPixmap();

    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &Blinky::updatePixmap);
    m_timer.start(ANIM_SPEED);

    m_weakAnim = new QPropertyAnimation(this, "weak", this);
    m_weakAnim->setStartValue(WEAK_MODE_SPEED);
    m_weakAnim->setDuration(WEAK_MODE_SPEED);
    m_weakAnim->setEndValue(0);

}

void Blinky::startWeakMode()
{
    m_weakAnim->start();
}

void Blinky::stopWeakMode()
{
    m_weakAnim->stop();
    m_weak = 0;
}

void Blinky::updatePixmap()
{
    if(getDirection() == Resources::Direction::Unset || !m_isAnimated)
    {
        setPixmap(m_thingsPixmap.copy(Resources::BLINKY_DOWN.x(),Resources::BLINKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
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
    m_currentIndex %= Resources::BLINKY_COUNT_ANIM_FRAMES;

}

void Blinky::setWeak(qreal val)
{
    m_weak = val;
}

qreal Blinky::weak() const
{
    return m_weak;
}

bool Blinky::isWeak()
{
    return m_weak > 0;
}
