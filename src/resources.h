#ifndef RESOURCES_H
#define RESOURCES_H
#include <QString>
#include <QPoint>
#include <QSize>
#include <QColor>

class Resources
{
public:
    Resources();

    enum class Direction
    {
        Up = 1,
        Down = -1,
        Left = 2,
        Right = -2,
        Unset = 0
    };

    constexpr static const QSize RESOLUTION = QSize(448, 596);
    constexpr static const QColor BG_COLOR = QColor(0, 0, 0);

    static void loadLabyrinth();

    static const QString PATH_TO_LABYRINTH_PIXMAP;
    constexpr static const unsigned int LABYRINTH_TILE_SIZE = 16;

    static const QString PATH_TO_THINGS_PIXMAP;
    constexpr static const unsigned int THINGS_TILE_SIZE = 30;



    constexpr static const unsigned int PACMAN_COUNT_ANIM_FRAMES = 3;
    constexpr static const QPoint PACMAN_UP = QPoint(0, 0);
    constexpr static const QPoint PACMAN_DOWN = QPoint(90, 0);
    constexpr static const QPoint PACMAN_LEFT = QPoint(180, 0);
    constexpr static const QPoint PACMAN_RIGHT = QPoint(270, 0);

    constexpr static const unsigned int BLINKY_COUNT_ANIM_FRAMES = 2;
    constexpr static const QPoint BLINKY_UP = QPoint(0, 30);
    constexpr static const QPoint BLINKY_DOWN = QPoint(60, 30);
    constexpr static const QPoint BLINKY_LEFT = QPoint(120, 30);
    constexpr static const QPoint BLINKY_RIGHT = QPoint(180, 30);

    constexpr static const unsigned int PINKY_COUNT_ANIM_FRAMES = 2;
    constexpr static const QPoint PINKY_UP = QPoint(0, 60);
    constexpr static const QPoint PINKY_DOWN = QPoint(60, 60);
    constexpr static const QPoint PINKY_LEFT = QPoint(120, 60);
    constexpr static const QPoint PINKY_RIGHT = QPoint(180, 60);

    constexpr static const unsigned int INKY_COUNT_ANIM_FRAMES = 2;
    constexpr static const QPoint INKY_UP = QPoint(0, 90);
    constexpr static const QPoint INKY_DOWN = QPoint(60, 90);
    constexpr static const QPoint INKY_LEFT = QPoint(120, 90);
    constexpr static const QPoint INKY_RIGHT = QPoint(180, 90);

    constexpr static const unsigned int CLYDE_COUNT_ANIM_FRAMES = 2;
    constexpr static const QPoint CLYDE_UP = QPoint(0, 120);
    constexpr static const QPoint CLYDE_DOWN = QPoint(60, 120);
    constexpr static const QPoint CLYDE_LEFT = QPoint(120, 120);
    constexpr static const QPoint CLYDE_RIGHT = QPoint(0, 120);

    constexpr static const unsigned int FRIGHTENED_GHOST_COUNT_ANIM_FRAMES = 2;
    constexpr static const QPoint FRIGHTENED_GHOST = QPoint(0, 120);

    constexpr static const unsigned int DEAD_PACMAN_COUNT_ANIM_FRAMES = 12;
    constexpr static const QPoint DEAD_PACMAN = QPoint(0, 150);

    constexpr static int FPS = 120;
};

#endif // RESOURCES_H
