greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/entity.cpp \
    src/gamescene.cpp \
    src/labyrinth.cpp \
    src/main.cpp \
    src/pacman.cpp \
    src/resources.cpp \
    src/view.cpp

HEADERS += \
    src/entity.h \
    src/gamescene.h \
    src/labyrinth.h \
    src/pacman.h \
    src/resources.h \
    src/view.h

RESOURCES += \
    resources.qrc
