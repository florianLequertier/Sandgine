#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T15:51:33
#
#-------------------------------------------------

QT       -= gui

TARGET = SandgineCore
TEMPLATE = lib
CONFIG = staticlib
CONFIG += c++11

DEFINES += SANDGINECORE_LIBRARY

SOURCES += sandginecore.cpp \
    carray.cpp \
    entity.cpp \
    component.cpp \
    baseworld.cpp \
    resourcefactory.cpp \
    prefabfactory.cpp \
    prefab.cpp \
    transformable.cpp \
    handler.cpp \
    componentfactory.cpp \
    sprite.cpp \
    singleton.cpp \
    worldobject.cpp

HEADERS += sandginecore.h \
    carray.h \
    entity.h \
    component.h \
    baseworld.h \
    resourcefactory.h \
    prefabfactory.h \
    prefab.h \
    transformable.h \
    handler.h \
    componentfactory.h \
    sprite.h \
    singleton.h \
    worldobject.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
