#include "gamescene.h"
#include "resources.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>
#include <QFontDatabase>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
      m_prizeCanGenerated(true), m_deltaTime(0.0f),
      m_loopTime(0.0f),
      m_loopSpeed(int(1000.0f/Resources::FPS) ),
      m_countOfWinPrize(0)
{
    srand(time(0));
    loadPixmap();
    loadSFX();
    setSceneRect(0,0, Resources::RESOLUTION.width(), Resources::RESOLUTION.height());
    setBackgroundBrush(QBrush(Resources::BG_COLOR));
    initLabyrinth();
    initGUI();
    initPrize();
    initPackman();
    initGhosts();

    renderLabyrinth();
    renderGhosts();
    renderPrize();
    renderPacman();
    renderGUI();

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
            m_pacman->setPos(m_pacman->getScreenPosX(), m_pacman->getScreenPosY());        }
        else
        {
            m_pacman->stop();
        }

        if(m_labyrinthObj.isIntersection(m_pacman->getTileX(), m_pacman->getTileY()))
        {
            m_pacman->stop();
        }

        if(m_labyrinthObj.removeDot(m_pacman, m_score))
        {
            m_packman_chompSFX.play();
        }

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
            if (!m_clyde->isScattering())
            {
                if (sqrt(pow((m_clyde->getTileX() - (m_pacman->getTileX())), 2) + pow((m_clyde->getTileY() - (m_pacman->getTileY())), 2)) < 9)
                {
                    m_clyde->setDestination(m_pacman->getTileX(), m_pacman->getTileY());
                }
                else
                {
                    m_clyde->setDestination(1, 32);
                }
            }
        }
        renderLabyrinth();
        teleportTunnels(m_pacman);
        teleportTunnels(m_blinky);
        teleportTunnels(m_inky);
        teleportTunnels(m_pinky);
        teleportTunnels(m_clyde);

        handleGhostMovement(m_blinky);
        handleGhostMovement(m_inky);
        handleGhostMovement(m_pinky);
        handleGhostMovement(m_clyde);

        if(m_pacman->getDotsEaten() == 10)
        {
            m_inky->teleport(13, 14);
            m_inky->setAnimated(true);
        }
        if(m_pacman->getDotsEaten() == 25)
        {
            m_pinky->teleport(13, 14);
            m_pinky->setAnimated(true);
        }
        if(m_pacman->getDotsEaten() == 75)
        {
            m_clyde->teleport(13, 14);
            m_clyde->setAnimated(true);
        }

        for(int i = 40; i <= 240; i+=40)
        {
            if(m_pacman->getDotsEaten() == i)
            {
                generatePrize();
            }
        }

        handleGhostFrightening(m_blinky);
        handleGhostFrightening(m_inky);
        handleGhostFrightening(m_pinky);
        handleGhostFrightening(m_clyde);

        checkCollisionWithPrize();

        m_blinky->setPos(m_blinky->getScreenPosX(), m_blinky->getScreenPosY());
        m_inky->setPos(m_inky->getScreenPosX(), m_inky->getScreenPosY());
        m_pinky->setPos(m_pinky->getScreenPosX(), m_pinky->getScreenPosY());
        m_clyde->setPos(m_clyde->getScreenPosX(), m_clyde->getScreenPosY());

        updateGUI();
    }
}

void GameScene::restart()
{
    if (m_blinky->isOutOfCage())
    {
        m_blinky->teleport(13, 14);
    }
    if (m_pinky->isOutOfCage())
    {
        m_pinky->teleport(15, 14);
    }
    if (m_inky->isOutOfCage())
    {
        m_inky->teleport(18, 14);
    }
    if (m_clyde->isOutOfCage())
    {
        m_clyde->teleport(9, 14);
    }
    m_pacman->teleport(13, 26);
    m_pacman->clearQueueDirection();
    m_pacman->setDead(false);
    m_lives--;
    if(m_lives < 0)
    {
        m_lives = 0;
    }
}

void GameScene::weakAllGhosts()
{
    m_blinky->startWeakMode();
    m_inky->startWeakMode();
    m_pinky->startWeakMode();
    m_clyde->startWeakMode();
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

    QPixmap thingsPixmap;
    if( thingsPixmap.load(Resources::PATH_TO_THINGS_PIXMAP) )
    {
        qDebug() << "Things is loaded SUCCESSFULLY";
        m_lifePacmanPixmap = thingsPixmap.copy(Resources::LIFE_PACKMAN.x(), Resources::LIFE_PACKMAN.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);

    }
    else
    {
        qFatal("Things is loaded SUCCESSFULLY");
    }

    m_strawberryPixmap = thingsPixmap.copy(Resources::STRAWBERRY.x(), Resources::STRAWBERRY.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_cherryPixmap = thingsPixmap.copy(Resources::CHERRY.x(), Resources::CHERRY.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_bellPixmap = thingsPixmap.copy(Resources::BELL.x(), Resources::BELL.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
    m_keyPixmap = thingsPixmap.copy(Resources::KEY.x(), Resources::KEY.y(), Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);

    m_possiblePrizesList.push_back(m_strawberryPixmap);
    m_possiblePrizesList.push_back(m_cherryPixmap);
    m_possiblePrizesList.push_back(m_bellPixmap);
    m_possiblePrizesList.push_back(m_keyPixmap);
}

void GameScene::loadSFX()
{
    m_packman_chompSFX.setSource(Resources::PACMAN_CHOMP_SFX);
    m_packman_chompSFX.setVolume(0.5f);
    m_packman_deathSFX.setSource(Resources::PACMAN_DEATH_SFX);
    m_packman_eatfruitSFX.setSource(Resources::PACMAN_EATFRUIT_SFX);
    m_packman_eatghostSFX.setSource(Resources::PACMAN_EATGHOST_SFX);
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

    m_clyde = new Clyde();
    m_clyde->setPos(m_clyde->getScreenPosX(), m_clyde->getScreenPosY());

}

void GameScene::initPrize()
{
    m_prize = new QGraphicsPixmapItem();
    m_prize->setPos(13 * Resources::LABYRINTH_TILE_SIZE + 8.0f + 8.0f - Resources::THINGS_TILE_SIZE/2, 20 * Resources::LABYRINTH_TILE_SIZE + 8.0f - Resources::THINGS_TILE_SIZE/2);
    m_prize->setPixmap(QPixmap());
}

void GameScene::initGUI()
{
    int id = QFontDatabase::addApplicationFont(Resources::PATH_TO_FONT);
    m_basicFont = QFont(QFontDatabase::applicationFontFamilies(id).at(0), 22 /*22pt ~ 30px */, 0);

    m_scoreTextItem = new QGraphicsSimpleTextItem();
    m_scoreTextItem->setBrush(QBrush(Resources::FONT_COLOR));
    m_scoreTextItem->setPen(QPen(Resources::FONT_COLOR));
    m_scoreTextItem->setPos(12*Resources::LABYRINTH_TILE_SIZE, 1*Resources::LABYRINTH_TILE_SIZE);
    m_scoreTextItem->setFont(m_basicFont);
    m_scoreTextItem->setText("Score: " + QString::number(m_score).right(5));

    m_lives = 6;
    for(int i = 0; i < m_lives; ++i)
    {
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();
        pixmapItem->setPixmap(m_lifePacmanPixmap);
        pixmapItem->setPos(i * Resources::THINGS_TILE_SIZE, 18.5 * Resources::THINGS_TILE_SIZE);
        m_livesPixmapItem.append(pixmapItem);
    }

    m_sizeOfPrize = 6;
    for(int i = 0; i < m_sizeOfPrize; ++i)
    {
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();
        pixmapItem->setPos(Resources::RESOLUTION.width() - (i+1) * Resources::THINGS_TILE_SIZE, 18.5 * Resources::THINGS_TILE_SIZE);
        m_prizesPixmapItem.append(pixmapItem);
    }
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
    addItem(m_clyde);
}

void GameScene::renderPrize()
{
    addItem(m_prize);
}

void GameScene::renderGUI()
{
    addItem(m_scoreTextItem);

    for(int i = 0; i < m_lives; ++i)
    {
        addItem(m_livesPixmapItem.at(i));
    }

    for(int i = 0; i < m_sizeOfPrize; ++i)
    {
        addItem(m_prizesPixmapItem.at(i));
    }
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
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() - 1);
            break;
        case Resources::Direction::Down:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX(), m_pacman->getTileY() + 1);
            break;
        case Resources::Direction::Left:
            return !m_labyrinthObj.tileBlocksEntity(m_pacman->getTileX() - 1, m_pacman->getTileY());
            break;
        case Resources::Direction::Right:
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
            ghost->stopWeakMode();
            addPoints(100);
            m_packman_eatghostSFX.play();
        }
        else if(!m_pacman->isDead())
        {
            m_pacman->setDead(true);
            m_packman_deathSFX.play();
            m_blinky->teleport(-2, -2);
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
            ghost->stopWeakMode();
            addPoints(100);
            m_packman_eatghostSFX.play();
        }
        else if(!m_pacman->isDead())
        {
            m_pacman->setDead(true);
            m_packman_deathSFX.play();
            m_inky->teleport(-2, -2);
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
            ghost->stopWeakMode();
            addPoints(100);
            m_packman_eatghostSFX.play();
        }
        else if(!m_pacman->isDead())
        {
            m_pacman->setDead(true);
            m_packman_deathSFX.play();
            m_pinky->teleport(-2, -2);
        }
    }
}

void GameScene::handleGhostFrightening(Clyde* ghost)
{
    if (m_pacman->getTileX() == ghost->getTileX() && m_pacman->getTileY() == ghost->getTileY())
    {
        if (ghost->isWeak())
        {
            ghost->teleport(13, 14);
            ghost->stopWeakMode();
            addPoints(100);
            m_packman_eatghostSFX.play();
        }
        else if(!m_pacman->isDead())
        {
            m_pacman->setDead(true);
            m_packman_deathSFX.play();
            m_clyde->teleport(-2, -2);
        }
    }
}

void GameScene::generatePrize()
{
    if(m_prizeCanGenerated)
    {
        m_prizeCanGenerated = false;
        m_prize->setPixmap(m_possiblePrizesList[rand() % Resources::COUNT_OF_PRIZES]);
    }
}

void GameScene::checkCollisionWithPrize()
{
    if (m_pacman->getTileX() == 13 && m_pacman->getTileY() == 20 && !m_prizeCanGenerated)
    {
        m_packman_eatfruitSFX.play();
        m_prizesPixmapItem[m_countOfWinPrize]->setPixmap(m_prize->pixmap());
        m_countOfWinPrize++;
        if(m_countOfWinPrize >= m_sizeOfPrize)
        {
            m_countOfWinPrize--;
        }
        m_prize->setPixmap(QPixmap());
        addPoints(50);
        m_prizeCanGenerated = true;
    }
}

void GameScene::addPoints(int n)
{
    m_score += n;
}

void GameScene::updateGUI()
{
    m_scoreTextItem->setText("Score: " + QString::number(m_score).right(5));

    for(int i = 0; i < 6; ++i)
    {
        m_livesPixmapItem[i]->hide();
    }
    for(int i = 0; i < m_lives; ++i)
    {
        m_livesPixmapItem[i]->show();
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
