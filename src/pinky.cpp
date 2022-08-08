#include "pinky.h"

void Pinky::loadPixmap()
{
    m_upPixmap = m_thingsPixmap.copy(Resources::PINKY_UP.x(), Resources::PINKY_UP.y(), Resources::PINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_rightPixmap = m_thingsPixmap.copy(Resources::PINKY_RIGHT.x(), Resources::PINKY_RIGHT.y(), Resources::PINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_downPixmap = m_thingsPixmap.copy(Resources::PINKY_DOWN.x(), Resources::PINKY_DOWN.y(), Resources::PINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_leftPixmap = m_thingsPixmap.copy(Resources::PINKY_LEFT.x(), Resources::PINKY_LEFT.y(), Resources::PINKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
}

Pinky::Pinky()
    : Ghost(13, 17, 26, 32), m_currentIndex(0), m_weak(0)
{
    m_speed = 0.7f;

    setPixmap(m_thingsPixmap.copy(Resources::PINKY_DOWN.x(),Resources::PINKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    loadPixmap();

    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &Pinky::updatePixmap);
    m_timer.start(ANIM_SPEED);

    m_weakAnim = new QPropertyAnimation(this, "weak", this);
    m_weakAnim->setStartValue(WEAK_MODE_SPEED);
    m_weakAnim->setDuration(WEAK_MODE_SPEED);
    m_weakAnim->setEndValue(0);

}

void Pinky::startWeakMode()
{
    m_weakAnim->start();
}

void Pinky::stopWeakMode()
{
    m_weakAnim->stop();
    m_weak = 0;
}

void Pinky::updatePixmap()
{
    if(getDirection() == Resources::Direction::Unset || !m_isAnimated)
    {
        setPixmap(m_thingsPixmap.copy(Resources::PINKY_DOWN.x(),Resources::PINKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
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
    m_currentIndex %= Resources::PINKY_COUNT_ANIM_FRAMES;

}

void Pinky::setWeak(qreal val)
{
    m_weak = val;
}

qreal Pinky::weak() const
{
    return m_weak;
}

bool Pinky::isWeak()
{
    return m_weak > 0;
}
