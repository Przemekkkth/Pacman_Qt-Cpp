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
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QSoundEffect>

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
    void loadSFX();
    void initLabyrinth();
    void initPackman();
    void initGhosts();
    void initPrize();
    void initGUI();

    void renderLabyrinth();
    void renderPacman();
    void renderGhosts();
    void renderPrize();
    void renderGUI();
    void saveScene();

    bool pacmanCanMove();
    bool ghostCanMove(Ghost* ghost);
    void teleportTunnels(Entity* entity);
    void handleGhostMovement(Ghost* ghost);
    float calculateDistance(Ghost* ghost, int addX, int addY);

    void handleGhostFrightening(Blinky* ghost);
    void handleGhostFrightening(Inky* ghost);
    void handleGhostFrightening(Pinky* ghost);
    void handleGhostFrightening(Clyde* ghost);

    void generatePrize();
    void checkCollisionWithPrize();

    void addPoints(int n);
    void updateGUI();
    //Visual Game ELEMENTS
    QPixmap m_labyrinthPixmap;
    QPixmap m_labyrinthPixmaps[32];
    QGraphicsPixmapItem* m_labyrinthPixmapItems[Labyrinth::LABYRINTH_WIDTH][Labyrinth::LABYRINTH_HEIGHT];
    Labyrinth m_labyrinthObj;
    Pacman* m_pacman;
    Blinky* m_blinky;
    Inky* m_inky;
    Pinky* m_pinky;
    Clyde* m_clyde;
    QGraphicsPixmapItem* m_prize;
    bool m_prizeCanGenerated, m_prizeIsActivated;
    //TO SERVER FRAME
    int currentFrame;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime, m_loopTime;
    const float m_loopSpeed;
    //GUI
    QFont m_basicFont;
    QGraphicsSimpleTextItem *m_scoreTextItem;
    QPixmap m_lifePacmanPixmap;
    int m_lives, m_score, m_sizeOfPrize, m_countOfWinPrize;
    QList<QGraphicsPixmapItem*> m_livesPixmapItem;
    QPixmap m_strawberryPixmap, m_cherryPixmap, m_bellPixmap, m_keyPixmap;
    QList<QPixmap> m_possiblePrizesList;
    QList<QGraphicsPixmapItem*> m_prizesPixmapItem;
    //SFX
    QSoundEffect m_packman_chompSFX, m_packman_deathSFX, m_packman_eatfruitSFX, m_packman_eatghostSFX;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
