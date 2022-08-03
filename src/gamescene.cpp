#include "gamescene.h"
#include "resources.h"

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}
{
    setSceneRect(0,0, Resources::RESOLUTION.width(), Resources::RESOLUTION.height());
}
