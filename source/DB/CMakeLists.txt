set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpDBShell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/RailwayNetDBShell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/UserDBShell.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/UserDBObject.cpp
        PARENT_SCOPE)
