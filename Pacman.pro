greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/gamescene.cpp \
    src/labyrinth.cpp \
    src/main.cpp \
    src/resources.cpp \
    src/view.cpp

HEADERS += \
    src/gamescene.h \
    src/labyrinth.h \
    src/resources.h \
    src/view.h

RESOURCES += \
    resources.qrc
