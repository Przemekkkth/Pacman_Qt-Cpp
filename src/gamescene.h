#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include "labyrinth.h"
#include "pacman.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
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
    void restart();
    void weakAllGhosts();
private:
    void loadPixmap();
    void initLabyrinth();
    void initPackman();
    void initGhosts();
    void renderLabyrinth();
    void renderPacman();
    void renderGhosts();
    void saveScene();
    bool pacmanCanMove();
    void teleportTunnels(Entity* entity);
    void handleGhostMovement(Ghost* ghost);
    float calculateDistance(Ghost* ghost, int addX, int addY);
    bool ghostCanMove(Ghost* ghost);
    void handleGhostFrightening(Blinky* ghost);
    void handleGhostFrightening(Inky* ghost);
    void handleGhostFrightening(Pinky* ghost);
    void handleGhostFrightening(Clyde* ghost);

    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];
    QGraphicsPixmapItem* m_labyrinthPixmapItems[Labyrinth::LABYRINTH_WIDTH][Labyrinth::LABYRINTH_HEIGHT];
    Labyrinth m_labyrinthObj;
    Pacman* m_pacman;
    Blinky* m_blinky;
    Inky* m_inky;
    Pinky* m_pinky;
    Clyde* m_clyde;
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
