#include "pacman.h"
#include <QKeyEvent>
Pacman::Pacman()
    : QGraphicsPixmapItem(), Entity(13, 26), m_dead(false), m_currentIndex(0)
{
    loadPixmap();
    //setPixmap(m_thingsPixmap.copy(0,0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    setPixmap(m_rightPixmap);
    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &Pacman::updatePixmap);
    m_timer.start(ANIM_SPEED);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

void Pacman::updatePixmap()
{
    if(m_directions.empty())
    {
        setPixmap(m_thingsPixmap.copy(0,0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
        return;
    }

    if(m_directions.front() == Resources::Direction::Up)
    {
        setPixmap(m_upPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    }
    else if(m_directions.front() == Resources::Direction::Right)
    {
        setPixmap(m_rightPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    }
    else if(m_directions.front() == Resources::Direction::Down)
    {
        setPixmap(m_downPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    }
    else if(m_directions.front() == Resources::Direction::Left)
    {
        setPixmap(m_leftPixmap.copy(m_currentIndex*Resources::THINGS_TILE_SIZE, 0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    }

    m_currentIndex += 1;
    m_currentIndex %= Resources::PACMAN_COUNT_ANIM_FRAMES;
}

void Pacman::loadPixmap()
{
    m_upPixmap    = m_thingsPixmap.copy(Resources::PACMAN_UP.x(), Resources::PACMAN_UP.y(), Resources::THINGS_TILE_SIZE*Resources::PACMAN_COUNT_ANIM_FRAMES, Resources::THINGS_TILE_SIZE);
    m_downPixmap  = m_thingsPixmap.copy(Resources::PACMAN_DOWN.x(), Resources::PACMAN_DOWN.y(), Resources::THINGS_TILE_SIZE*Resources::PACMAN_COUNT_ANIM_FRAMES, Resources::THINGS_TILE_SIZE);
    m_leftPixmap  = m_thingsPixmap.copy(Resources::PACMAN_LEFT.x(), Resources::PACMAN_LEFT.y(), Resources::THINGS_TILE_SIZE*Resources::PACMAN_COUNT_ANIM_FRAMES, Resources::THINGS_TILE_SIZE);
    m_rightPixmap = m_thingsPixmap.copy(Resources::PACMAN_RIGHT.x(), Resources::PACMAN_RIGHT.y(), Resources::THINGS_TILE_SIZE*Resources::PACMAN_COUNT_ANIM_FRAMES, Resources::THINGS_TILE_SIZE);

}

void Pacman::queueDirection(Resources::Direction dir)
{
    if (!m_directions.empty())
    {
        if (int(dir) == -int(m_directions.front()))
        {
            std::queue<Resources::Direction> clear;
            std::swap(m_directions, clear);
        }
    }

    if(m_directions.size() < 2)
        m_directions.push(dir);
}

void Pacman::move()
{
    if (!m_directions.empty())
    {
        switch (m_directions.front())
        {
        case Resources::Direction::Up:
            Entity::move(0, -SPEED);
            break;
        case Resources::Direction::Down:
            Entity::move(0, SPEED);
            break;
        case Resources::Direction::Left:
            Entity::move(-SPEED, 0);
            break;
        case Resources::Direction::Right:
            Entity::move(SPEED, 0);
            break;
        case Resources::Direction::Unset:

            break;
        }
    }
}

std::queue<Resources::Direction> Pacman::getDirections()
{
    return m_directions;
}

void Pacman::stop()
{
    if (m_directions.size() > 1)
    {
        if ((int)(m_screenPosX + 8 + Resources::THINGS_TILE_SIZE/2) % 16 == 0 && (int)(m_screenPosY + 8 + Resources::THINGS_TILE_SIZE/2) % 16 == 0)
        {
            switch (m_directions.front())
            {
            case Resources::Direction::Up:
                m_directions.pop();
                break;
            case Resources::Direction::Down:
                m_directions.pop();
                break;
            case Resources::Direction::Left:
                m_directions.pop();
                break;
            case Resources::Direction::Right:
                m_directions.pop();
                break;
            case Resources::Direction::Unset:
                m_directions.pop();
                break;
            }
        }
    }
}

void Pacman::eatDot()
{
    m_eatenDots++;
}

int Pacman::getDotsEaten()
{
    return m_eatenDots;
}

void Pacman::setDead(bool d)
{
    m_dead = d;
}

bool Pacman::isDead()
{
    return m_dead;
}

void Pacman::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:
        queueDirection(Resources::Direction::Up);
        break;
    case Qt::Key_Right:
        queueDirection(Resources::Direction::Right);
        break;
    case Qt::Key_Down:
        queueDirection(Resources::Direction::Down);
        break;
    case Qt::Key_Left:
        queueDirection(Resources::Direction::Left);
        break;
    }
    QGraphicsPixmapItem::keyPressEvent(event);
}
