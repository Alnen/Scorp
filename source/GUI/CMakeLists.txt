cmake_minimum_required(VERSION 2.8.8)

find_package(Qt5Widgets REQUIRED)

set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/mainwindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/StationsList/StationsListModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/StationsList/TreeModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/StationsList/TreeModelItem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/MapScene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/MapSceneStyle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/TransitionGraphicsObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/StateGraphicsObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/PointGraphicsObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/LinkGraphicsObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/GraphicsObjectsGroup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/MarkerObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/TrackGraphicsObject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Map/MarkerCommandQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/UserInfo.cpp
        # icon.rc
        PARENT_SCOPE)

set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Scorp/GUI/mainwindow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Scorp/GUI/Map/MapScene.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Scorp/GUI/Map/PointGraphicsObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Scorp/GUI/Map/StateGraphicsObject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Scorp/GUI/StationsList/TreeModel.h)

qt5_wrap_cpp(MOC ${HEADERS})
set(MOC ${MOC} PARENT_SCOPE)
