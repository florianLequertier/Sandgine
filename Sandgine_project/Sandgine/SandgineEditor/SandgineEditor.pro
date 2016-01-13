#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T15:52:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SandgineEditor
TEMPLATE = app


SOURCES += main.cpp\
        editorwindow.cpp

HEADERS  += editorwindow.h

FORMS    += editorwindow.ui

INCLUDEPATH += ../SandgineCore
LIBS += -L../SandgineCore -lSandgineCore
