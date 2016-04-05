#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T14:15:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scorp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Map/GraphObject.cpp \
    StationsList/StationsListModel.cpp \
    StationsList/TreeModel.cpp \
    StationsList/TreeModelItem.cpp \
    Map/MapScene.cpp \
    Map/TransitionGraphicsObject.cpp \
    Map/StateGraphicsObject.cpp \
    Map/TrackGraphicsObject.cpp \
    Map/PointGraphicsObject.cpp

HEADERS  += mainwindow.h \
    Map/GraphObject.h \
    StationsList/StationsListModel.h \
    StationsList/TreeModel.h \
    StationsList/TreeModelItem.h \
    Map/MapScene.h \
    Map/TransitionGraphicsObject.h \
    Map/StateGraphicsObject.h \
    Map/TrackGraphicsObject.h \
    Map/PointGraphicsObject.h

RC_FILE += icon.rc
