TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lncurses

#include sfml
CONFIG += link_pkgconfig
PKGCONFIG += sfml-all

SOURCES += \
        main.cpp \
        map.cpp \
        output.cpp \
        player.cpp \
        raycasting.cpp

HEADERS += \
    map.h \
    output.h \
    player.h \
    raycasting.h
