#include "inky.h"


void Inky::loadPixmap()
{
    m_upPixmap = m_thingsPixmap.copy(Resources::INKY_UP.x(), Resources::INKY_UP.y(), Resources::INKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_rightPixmap = m_thingsPixmap.copy(Resources::INKY_RIGHT.x(), Resources::INKY_RIGHT.y(), Resources::INKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_downPixmap = m_thingsPixmap.copy(Resources::INKY_DOWN.x(), Resources::INKY_DOWN.y(), Resources::INKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_leftPixmap = m_thingsPixmap.copy(Resources::INKY_LEFT.x(), Resources::INKY_LEFT.y(), Resources::INKY_COUNT_ANIM_FRAMES*Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
}

Inky::Inky()
    : Ghost(11, 17, 26, 32), m_currentIndex(0), m_weak(0)
{
    setPixmap(m_thingsPixmap.copy(Resources::INKY_DOWN.x(),Resources::INKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    loadPixmap();

    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &Inky::updatePixmap);
    m_timer.start(ANIM_SPEED);

    m_weakAnim = new QPropertyAnimation(this, "weak", this);
    m_weakAnim->setStartValue(WEAK_MODE_SPEED);
    m_weakAnim->setDuration(WEAK_MODE_SPEED);
    m_weakAnim->setEndValue(0);

}

void Inky::startWeakMode()
{
    m_weakAnim->start();
}

void Inky::stopWeakMode()
{
    m_weakAnim->stop();
    m_weak = 0;
}

void Inky::updatePixmap()
{
    if(getDirection() == Resources::Direction::Unset || !m_isAnimated)
    {
        setPixmap(m_thingsPixmap.copy(Resources::INKY_DOWN.x(),Resources::INKY_DOWN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
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
    m_currentIndex %= Resources::INKY_COUNT_ANIM_FRAMES;

}

void Inky::setWeak(qreal val)
{
    m_weak = val;
}

qreal Inky::weak() const
{
    return m_weak;
}

bool Inky::isWeak()
{
    return m_weak > 0;
}
