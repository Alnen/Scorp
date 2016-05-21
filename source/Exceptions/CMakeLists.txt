set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpCoreException.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpDBException.cpp
        PARENT_SCOPE)
