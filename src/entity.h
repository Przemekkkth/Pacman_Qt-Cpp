#ifndef ENTITY_H
#define ENTITY_H
#include <QPixmap>
class Entity
{

public:
    Entity(int tilePosX, int tilePosY);

    float getScreenPosX();
    float getScreenPosY();
    int getTileX();
    int getTileY();

    void move(float x, float y);
    void teleport(int x, int y);
protected:
    float m_screenPosX;
    float m_screenPosY;
    int m_tileX;
    int m_tileY;
    const QPixmap m_thingsPixmap;
};

#endif // ENTITY_H
