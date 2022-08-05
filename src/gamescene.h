#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "labyrinth.h"
#include "pacman.h"
#include <QPixmap>
#include <QTimer>
#include <QElapsedTimer>

class QGraphicsPixmapItem;
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:
private slots:
    void loop();
private:
    void loadPixmap();
    void initLabyrinth();
    void initPackman();
    void renderLabyrinth();
    void renderPacman();
    void saveScene();
    bool pacmanCanMove();
    void teleportTunnels(Entity* entity);

    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];
    QGraphicsPixmapItem* m_labyrinthPixmapItems[Labyrinth::LABYRINTH_WIDTH][Labyrinth::LABYRINTH_HEIGHT];
    Labyrinth m_labyrinthObj;
    Pacman* m_pacman;
    // QGraphicsScene interface
    int currentFrame;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
