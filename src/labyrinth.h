#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <QObject>

class Labyrinth
{
public:
    explicit Labyrinth();

    static const uint LABYRINTH_WIDTH  = 28;
    static const uint LABYRINTH_HEIGHT = 36;

    uint tiles(int x, int y) const;
    bool tileBlocksEntity(int x, int y);
private:
    uint m_tiles[LABYRINTH_WIDTH][LABYRINTH_HEIGHT];
};

#endif // LABYRINTH_H
