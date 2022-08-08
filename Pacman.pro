greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/clyde.cpp \
    src/blinky.cpp \
    src/entity.cpp \
    src/gamescene.cpp \
    src/ghost.cpp \
    src/inky.cpp \
    src/labyrinth.cpp \
    src/main.cpp \
    src/pacman.cpp \
    src/pinky.cpp \
    src/resources.cpp \
    src/view.cpp

HEADERS += \
    src/clyde.h \
    src/blinky.h \
    src/entity.h \
    src/gamescene.h \
    src/ghost.h \
    src/inky.h \
    src/labyrinth.h \
    src/pacman.h \
    src/pinky.h \
    src/resources.h \
    src/view.h

RESOURCES += \
    resources.qrc
