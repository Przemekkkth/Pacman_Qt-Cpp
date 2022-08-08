#include "gamescene.h"
#include "resources.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
      m_deltaTime(0.0f), m_loopTime(0.0f),
      m_loopSpeed(int(1000.0f/Resources::FPS) )
{
    loadPixmap();
    setSceneRect(0,0, Resources::RESOLUTION.width(), Resources::RESOLUTION.height());
    setBackgroundBrush(QBrush(Resources::BG_COLOR));
    initLabyrinth();
    initPackman();
    initGhosts();
    renderLabyrinth();
    renderGhosts();
    renderPacman();


    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    connect(m_pacman, &Pacman::deadAnimOver, this, &GameScene::restart);
    connect(&m_labyrinthObj, &Labyrinth::bigDotIsEaten, this, &GameScene::weakAllGhosts);
    m_timer.start(int(1000.0f/Resources::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    if( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        if (pacmanCanMove() && !m_pacman->isDead())
        {
            m_pacman->move();
            m_pacman->setPos(m_pacman->getScreenPosX(), m_pacman->getScreenPosY());
            qDebug() << "Move" << m_pacman->getScreenPosX() << " : " << m_pacman->getScreenPosY();
        }
        else
        {
            //qDebug() << "stop";
            m_pacman->stop();
        }

        if(m_labyrinthObj.isIntersection(m_pacman->getTileX(), m_pacman->getTileY()))
        {
            m_pacman->stop();
        }

        m_labyrinthObj.removeDot(m_pacman, m_blinky, m_inky, m_pinky, nullptr);

        if (!m_pacman->getDirections().empty())
        {
            if(!m_blinky->isScattering())
            {
                m_blinky->setDestination(m_pacman->getTileX(), m_pacman->getTileY());
            }
            if (!m_pinky->isScattering())
            {
                switch (m_pacman->getDirections().front())
                {
                case Resources::Direction::Up:
                    m_pinky->setDestination(m_pacman->getTileX(), m_pacman->getTileY() - 4);
                    break;
                case Resources::Direction::Down:
                    m_pinky->setDestination(m_pacman->getTileX(), m_pacman->getTileY() + 4);
                    break;
                case Resources::Direction::Left:
                    m_pinky->setDestination(m_pacman->getTileX() - 4, m_pacman->getTileY());
                    break;
                case Resources::Direction::Right:
                    m_pinky->setDestination(m_pacman->getTileX() + 4, m_pacman->getTileY());
                    break;
                }
            }
            if(!m_inky->isScattering())
            {
                m_inky->setDestination(m_pacman->getTileX() + (m_blinky->getTileX() - m_pacman->getTileX()), m_pacman->getTileY() + (m_blinky->getTileY() - m_pacman->getTileY()));
            }
        }
        renderLabyrinth();
        teleportTunnels(m_pacman);
        teleportTunnels(m_blinky);
        teleportTunnels(m_inky);
        teleportTunnels(m_pinky);

        handleGhostMovement(m_blinky);
        handleGhostMovement(m_inky);
        handleGhostMovement(m_pinky);

        if(m_pacman->getDotsEaten() == 10)
        {
            m_inky->teleport(13, 14);
            m_inky->setAnimated(true);
            //m_inky->setPosition(13, 14);
        }
        if(m_pacman->getDotsEaten() == 25)
        {
            m_pinky->teleport(13, 14);
            m_pinky->setAnimated(true);
            //m_pinky->setPos(m_pinky->getScreenPosX(), m_pinky->getScreenPosY());
        }



        handleGhostFrightening(m_blinky);
        handleGhostFrightening(m_inky);
        handleGhostFrightening(m_pinky);

        m_blinky->setPos(m_blinky->getScreenPosX(), m_blinky->getScreenPosY());
        m_inky->setPos(m_inky->getScreenPosX(), m_inky->getScreenPosY());
        m_pinky->setPos(m_pinky->getScreenPosX(), m_pinky->getScreenPosY());
    }
}

void GameScene::restart()
{
    if (m_blinky->isOutOfCage()) m_blinky->teleport(13, 14);
    if (m_pinky->isOutOfCage()) m_pinky->teleport(15, 14);
    if (m_inky->isOutOfCage()) m_inky->teleport(18, 14);
//    if (clyde->isOutOfCage()) clyde->teleport(13, 14);
    m_pacman->teleport(13, 26);
    m_pacman->clearQueueDirection();
    m_pacman->setDead(false);
}

void GameScene::weakAllGhosts()
{
    m_blinky->startWeakMode();
    m_inky->startWeakMode();
    m_pinky->startWeakMode();
}

void GameScene::loadPixmap()
{
    if( m_labyrinthPixmap.load(Resources::PATH_TO_LABYRINTH_PIXMAP) )
    {
        qDebug() << "Labyrinth is loaded SUCCESSFULLY";
    }
    else
    {
        qFatal("Labyrinth is loaded SUCCESSFULLY");
    }


}

void GameScene::initLabyrinth()
{
    int index = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_labyrinthPixmaps[index] = m_labyrinthPixmap.copy(i * Resources::LABYRINTH_TILE_SIZE, j * Resources::LABYRINTH_TILE_SIZE, Resources::LABYRINTH_TILE_SIZE, Resources::LABYRINTH_TILE_SIZE);
            index++;
        }
    }
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        for (int j = 0; j < int(Labyrinth::LABYRINTH_HEIGHT); j++)
        {
            m_labyrinthPixmapItems[i][j] =  new QGraphicsPixmapItem();
            m_labyrinthPixmapItems[i][j]->setPixmap(m_labyrinthPixmaps[ m_labyrinthObj.tiles(i, j) ]);
            m_labyrinthPixmapItems[i][j]->setPos(i*Resources::LABYRINTH_TILE_SIZE, j*Resources::LABYRINTH_TILE_SIZE);
            addItem(m_labyrinthPixmapItems[i][j]);
        }
    }

#ifdef QT_DEBUG
    QPen linePen((QColor(Qt::white)));
    linePen.setWidth(1);
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem();
        lineItem->setLine(i*Resources::LABYRINTH_TILE_SIZE, 0,
                          i*Resources::LABYRINTH_TILE_SIZE, Resources::RESOLUTION.height());
        lineItem->setPen(linePen);
        addItem(lineItem);
    }
    for (int i = 0; i < int(Labyrinth::LABYRINTH_HEIGHT); i++)
    {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem();
        lineItem->setLine(0, i * Resources::LABYRINTH_TILE_SIZE,
                          Resources::RESOLUTION.width(), i * Resources::LABYRINTH_TILE_SIZE);
        lineItem->setPen(linePen);
        addItem(lineItem);
    }

#endif
}

void GameScene::initPackman()
{
    m_pacman = new Pacman();
    m_pacman->setFocus();
    m_pacman->setPos(m_pacman->getScreenPosX(), m_pacman->getScreenPosY());
}

void GameScene::initGhosts()
{
    m_blinky = new Blinky();
    m_blinky->teleport(13, 14);
    m_blinky->setPos(m_blinky->getScreenPosX(), m_blinky->getScreenPosY());
    m_blinky->setAnimated(true);

    m_inky = new Inky();
    m_inky->setPos(m_inky->getScreenPosX(), m_inky->getScreenPosY());

    m_pinky = new Pinky();
    m_pinky->setPos(m_pinky->getScreenPosX(), m_pinky->getScreenPosY());

}

void GameScene::renderLabyrinth()
{
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        for (int j = 0; j < int(Labyrinth::LABYRINTH_HEIGHT); j++)
        {
            m_labyrinthPixmapItems[i][j]->setPixmap(m_labyrinthPixmaps[ m_labyrinthObj.tiles(i, j) ]);
        }
    }
}

void GameScene::renderPacman()
{
    addItem(m_pacman);
}

void GameScene::renderGhosts()
{
    addItem(m_blinky);
    addItem(m_inky);
    addItem(m_pinky);
}

void GameScene::saveScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

bool GameScene::pacmanCanMove()
{
    if (!m_pacman->getDirections().empty())
    {
        switch (m_pacman->getDirections().front())
        {
        case Resources::Direction::Up:
            qDebug() << "UP " << m_labyrinthObj.tiles(m_pacman->getTileX(), m_pacman->getTileY() - 1);
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() - 1);
            break;
        case Resources::Direction::Down:
            qDebug() << "Down " << m_labyrinthObj.tiles(m_pacman->getTileX(), m_pacman->getTileY() + 1);
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() + 1);
            break;
        case Resources::Direction::Left:
            qDebug() << "Left " << m_pacman->getTileX() - 1 << " " << m_pacman->getTileY();
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() - 1, m_pacman->getTileY());
            break;
        case Resources::Direction::Right:
            qDebug() << "Right " << m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() + 1, m_pacman->getTileY());
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() + 1, m_pacman->getTileY());
            break;
        }
    }
    return true;
}

void GameScene::teleportTunnels(Entity* entity)
{
    if (entity->getTileX() == 0 && entity->getTileY() == 17)
        entity->teleport(26, 17);
    else if (entity->getTileX() == 27 && entity->getTileY() == 17)
        entity->teleport(1, 17);
}

void GameScene::handleGhostMovement(Ghost* ghost)
{
    if (ghost->isScattering())
    {
        if (ghost->getTileX() == ghost->getDestX() && ghost->getTileY() == ghost->getDestY())
        {
            ghost->setScattering(false);
        }
    }

    if (m_labyrinthObj.isIntersection(ghost->getTileX(), ghost->getTileY()))
    {
        if (ghost->shouldTakeDecision())
        {
            float dRight = calculateDistance(ghost, 1, 0);
            float dLeft = calculateDistance(ghost, -1, 0);
            float dUp = calculateDistance(ghost, 0, -1);
            float dDown = calculateDistance(ghost, 0, 1);

            if (dRight < dLeft && dRight < dUp && dRight < dDown)
                ghost->setDirection(Resources::Direction::Right);
            else if (dLeft < dRight && dLeft < dUp && dLeft < dDown)
                ghost->setDirection(Resources::Direction::Left);
            else if (dUp < dLeft && dUp < dRight && dUp < dDown)
                ghost->setDirection(Resources::Direction::Up);
            else if (dDown < dLeft && dDown < dUp && dDown < dRight)
                ghost->setDirection(Resources::Direction::Down);
        }
        ghost->setTakeDecision(false);
    }
    else
    {
        ghost->setTakeDecision(true);
    }
    if (ghostCanMove(ghost) && ghost->isOutOfCage())
        ghost->move();
    else
        ghost->setTakeDecision(true);
}

float GameScene::calculateDistance(Ghost *ghost, int addX, int addY)
{
    float distance = 1000000.0f;
    if (!m_labyrinthObj.tileBlocksEntity(ghost->getTileX() + addX, ghost->getTileY() + addY))
    {
        //
        distance = (float) sqrt(pow((ghost->getDestX() - (ghost->getTileX() + addX)), 2) + pow((ghost->getDestY() - (ghost->getTileY() + addY)), 2));
    }
    return distance;
}

bool GameScene::ghostCanMove(Ghost *ghost)
{
    switch (ghost->getDirection())
    {
    case Resources::Direction::Up:
        return !m_labyrinthObj.tileBlocksEntity(ghost->getTileX(), ghost->getTileY() - 1);
        break;
    case Resources::Direction::Down:
        return !m_labyrinthObj.tileBlocksEntity(ghost->getTileX(), ghost->getTileY() + 1);
        break;
    case Resources::Direction::Left:
        return !m_labyrinthObj.tileBlocksEntity(ghost->getTileX() - 1, ghost->getTileY());
        break;
    case Resources::Direction::Right:
        return !m_labyrinthObj.tileBlocksEntity(ghost->getTileX() + 1, ghost->getTileY());
        break;
    default:
        return false;
    }
}

void GameScene::handleGhostFrightening(Blinky *ghost)
{
    if (m_pacman->getTileX() == ghost->getTileX() && m_pacman->getTileY() == ghost->getTileY())
    {
        if (ghost->isWeak())
        {
            ghost->teleport(13, 14);
            ghost->setFrightened(false);
            ghost->stopWeakMode();
        }
        else
        {
            m_pacman->setDead(true);
            //m_blinky->setDirection(Resources::Direction::Unset);
//            m_pacman->clearQueueDirection();
            m_blinky->teleport(-2, -2);
//			pinky->teleport(-2, -2);
//			inky->teleport(-2, -2);
//			clyde->teleport(-2, -2);
        }
    }
}

void GameScene::handleGhostFrightening(Inky* ghost)
{
    if (m_pacman->getTileX() == ghost->getTileX() && m_pacman->getTileY() == ghost->getTileY())
    {
        if (ghost->isWeak())
        {
            ghost->teleport(13, 14);
            ghost->setFrightened(false);
            ghost->stopWeakMode();
        }
        else
        {
            m_pacman->setDead(true);
            //m_blinky->setDirection(Resources::Direction::Unset);
//            m_pacman->clearQueueDirection();
            m_inky->teleport(-2, -2);
//			pinky->teleport(-2, -2);
//			inky->teleport(-2, -2);
//			clyde->teleport(-2, -2);
        }
    }
}

void GameScene::handleGhostFrightening(Pinky* ghost)
{
    if (m_pacman->getTileX() == ghost->getTileX() && m_pacman->getTileY() == ghost->getTileY())
    {
        if (ghost->isWeak())
        {
            ghost->teleport(13, 14);
            ghost->setFrightened(false);
            ghost->stopWeakMode();
        }
        else
        {
            m_pacman->setDead(true);
            //m_blinky->setDirection(Resources::Direction::Unset);
//            m_pacman->clearQueueDirection();
            m_pinky->teleport(-2, -2);
//			pinky->teleport(-2, -2);
//			inky->teleport(-2, -2);
//			clyde->teleport(-2, -2);
        }
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Z:
        saveScene();
        break;
    }

    QGraphicsScene::keyPressEvent(event);
}
