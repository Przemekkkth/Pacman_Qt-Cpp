#include "resources.h"
#include <QDebug>

const QString Resources::PATH_TO_LABYRINTH_PIXMAP = ":/res/sprite/labyrinth.png";
const QString Resources::PATH_TO_THINGS_PIXMAP    = ":/res/sprite/things.png";
const QString Resources::PATH_TO_FONT             = ":/res/font/arcadepi.ttf";

const QUrl Resources::PACMAN_CHOMP_SFX            = QUrl("qrc:/res/sfx/pacman_chomp.wav");
const QUrl Resources::PACMAN_DEATH_SFX            = QUrl("qrc:/res/sfx/pacman_death.wav");
const QUrl Resources::PACMAN_EATFRUIT_SFX         = QUrl("qrc:/res/sfx/pacman_eatfruit.wav");
const QUrl Resources::PACMAN_EATGHOST_SFX         = QUrl("qrc:/res/sfx/pacman_eatghost.wav");

Resources::Resources()
{

}

