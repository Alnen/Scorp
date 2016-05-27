cmake_minimum_required(VERSION 2.8.8)

set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)

set(SOURCE_FILES
        ${SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpCore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpRoute.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/source/ScorpTime.cpp
        PARENT_SCOPE)