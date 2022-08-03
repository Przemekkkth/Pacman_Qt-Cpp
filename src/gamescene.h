#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "labyrinth.h"
#include <QPixmap>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:

private:
    void loadPixmap();
    void initLabyrinth();
    void renderLabyrinth();

    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];
    Labyrinth m_labyrinthObj;
};

#endif // GAMESCENE_H
