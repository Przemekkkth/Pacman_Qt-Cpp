greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/gamescene.cpp \
    src/main.cpp \
    src/resources.cpp \
    src/view.cpp

HEADERS += \
    src/gamescene.h \
    src/resources.h \
    src/view.h

RESOURCES += \
    resources.qrc
