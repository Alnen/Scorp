#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T14:15:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scorp
TEMPLATE = app


SOURCES += GUI/main.cpp\
    GUI/StationsList/StationsListModel.cpp \
    GUI/StationsList/TreeModel.cpp \
    GUI/StationsList/TreeModelItem.cpp \
    GUI/Map/MapScene.cpp \
    GUI/Map/TransitionGraphicsObject.cpp \
    GUI/Map/StateGraphicsObject.cpp \
    GUI/Map/PointGraphicsObject.cpp \
    GUI/UserInfo.cpp \
    GUI/Map/LinkGraphicsObject.cpp \
    GUI/Map/GraphicsObjectsGroup.cpp \
    DB/ScorpDB.cpp \
    DB/ScorpDBSell.cpp \
    DB/sqlite/sqlite3.c \
    DB/sqlite/src/Backup.cpp \
    DB/sqlite/src/Column.cpp \
    DB/sqlite/src/Database.cpp \
    DB/sqlite/src/Statement.cpp \
    DB/sqlite/src/Transaction.cpp \
    GUI/Map/MarkerObject.cpp \
    GUI/Map/TrackGraphicsObject.cpp \
    GUI/mainwindow.cpp \

HEADERS  += GUI/mainwindow.h \
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
    GUI/Map/GraphicsObjectsGroup.h \
    GUI/Map/LinkGraphicsObject.h \
    GUI/Map/MapScene.h \
    GUI/Map/MarkerObject.h \
    GUI/Map/PointGraphicsObject.h \
    GUI/Map/StateGraphicsObject.h \
    GUI/Map/TrackGraphicsObject.h \
    GUI/Map/TransitionGraphicsObject.h \
    GUI/StationsList/StationsListModel.h \
    GUI/StationsList/TreeModel.h \
    GUI/StationsList/TreeModelItem.h \
    GUI/UserInfo.h

RC_FILE += GUI/icon.rc
