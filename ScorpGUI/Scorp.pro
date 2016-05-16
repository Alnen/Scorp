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
    StationsList/StationsListModel.cpp \
    StationsList/TreeModel.cpp \
    StationsList/TreeModelItem.cpp \
    Map/MapScene.cpp \
    Map/TransitionGraphicsObject.cpp \
    Map/StateGraphicsObject.cpp \
    Map/TrackGraphicsObject.cpp \
    Map/PointGraphicsObject.cpp \
    UserInfo.cpp \
    Map/LinkGraphicsObject.cpp \
    Map/GraphicsObjectsGroup.cpp \
    DB/ScorpDB.cpp \
    DB/ScorpDBSell.cpp \
    DB/sqlite/sqlite3.c \
    DB/sqlite/src/Backup.cpp \
    DB/sqlite/src/Column.cpp \
    DB/sqlite/src/Database.cpp \
    DB/sqlite/src/Statement.cpp \
    DB/sqlite/src/Transaction.cpp \
    Map/MarkerObject.cpp

HEADERS  += mainwindow.h \
    StationsList/StationsListModel.h \
    StationsList/TreeModel.h \
    StationsList/TreeModelItem.h \
    Map/MapScene.h \
    Map/TransitionGraphicsObject.h \
    Map/StateGraphicsObject.h \
    Map/TrackGraphicsObject.h \
    Map/PointGraphicsObject.h \
    UserInfo.h \
    Map/LinkGraphicsObject.h \
    Map/GraphicsObjectsGroup.h \
    DB/ScorpDBInterface.h \
    DB/ScorpDBSell.h \
    DB/Tables.h \
    DB/ScorpExceptions.h \
    DB/sqlite/include/Assertion.h \
    DB/sqlite/include/Backup.h \
    DB/sqlite/include/Column.h \
    DB/sqlite/include/Database.h \
    DB/sqlite/include/Exception.h \
    DB/sqlite/include/SQLiteCpp.h \
    DB/sqlite/include/Statement.h \
    DB/sqlite/include/Transaction.h \
    DB/sqlite/sqlite3.h \
    Map/MarkerObject.h

RC_FILE += icon.rc
