#include "gamescene.h"
#include "resources.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}
{
    loadPixmap();
    setSceneRect(0,0, Resources::RESOLUTION.width(), Resources::RESOLUTION.height());
    initLabyrinth();
    renderLabyrinth();
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
}

void GameScene::renderLabyrinth()
{
    for (int i = 0; i < int(Labyrinth::LABYRINTH_WIDTH); i++)
    {
        for (int j = 0; j < int(Labyrinth::LABYRINTH_HEIGHT); j++)
        {
            QGraphicsPixmapItem *labyrinthTileItem = new QGraphicsPixmapItem(m_labyrinthPixmaps[ m_labyrinthObj.tiles(i, j) ]);
            labyrinthTileItem->setPos(i*Resources::LABYRINTH_TILE_SIZE, j*Resources::LABYRINTH_TILE_SIZE);
            addItem(labyrinthTileItem);
        }
    }
}
