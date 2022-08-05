#ifndef LABYRINTH_H
#define LABYRINTH_H

#include <QObject>

class Pacman;
class Ghost;
class Labyrinth
{
public:
    explicit Labyrinth();

    static const uint LABYRINTH_WIDTH  = 28;
    static const uint LABYRINTH_HEIGHT = 36;

    uint tiles(int x, int y) const;
    bool tileBlocksEntity(int x, int y);
    bool isIntersection(int x, int y);
    void removeDot(Pacman* pacman, Ghost* ghost1, Ghost* ghost2, Ghost* ghost3, Ghost* ghost4);
private:
    uint m_tiles[LABYRINTH_WIDTH][LABYRINTH_HEIGHT];
    static constexpr int NONE_FIELD = 30;
    static constexpr int SMALL_DOT_FIELD = 26;
    static constexpr int BIG_DOT_FIELD = 27;
};

#endif // LABYRINTH_H
