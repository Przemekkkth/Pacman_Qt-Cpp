#include "pacman.h"

Pacman::Pacman()
    : QGraphicsPixmapItem(), Entity(13, 26)
{
    setPixmap(m_thingsPixmap.copy(0,0, Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE));
    setTransformOriginPoint(Resources::THINGS_TILE_SIZE, Resources::THINGS_TILE_SIZE);
}
