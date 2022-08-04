#include "entity.h"
#include <cmath>


Entity::Entity(int tilePosX, int tilePosY)
{
    m_tileX = tilePosX;
    m_tileY = tilePosY;
    m_screenPosX = tilePosX * 16.0f + 8.0f + 8.0f;
    m_screenPosY = tilePosY * 16.0f + 8.0f;
}

float Entity::getScreenPosX()
{
    return m_screenPosX;
}

float Entity::getScreenPosY()
{
    return m_screenPosY;
}

int Entity::getTileX()
{
    return m_tileX;
}

int Entity::getTileY()
{
    return m_tileY;
}

void Entity::move(float x, float y)
{
    screenPosX += x;
    screenPosY += y;

    if ((int)(screenPosX + 8) % 16 == 0 && (int)(screenPosY + 8) % 16 == 0)
    {
        tileX = (int) round((screenPosX - 8) / 16);
        tileY = (int) round((screenPosY - 8) / 16);
    }
}

void Entity::teleport(int x, int y)
{
    tileX = x;
    tileY = y;
    screenPosX = x * 16.0f + 8.0f;
    screenPosY = y * 16.0f + 8.0f;
}
