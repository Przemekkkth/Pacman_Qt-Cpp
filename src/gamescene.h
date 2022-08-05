#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "labyrinth.h"
#include "pacman.h"
#include <QPixmap>
#include <QTimer>

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
    void renderPacman();
    void saveScene();

    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];\
    Labyrinth m_labyrinthObj;
    Pacman m_packman;
    // QGraphicsScene interface
    int currentFrame;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
