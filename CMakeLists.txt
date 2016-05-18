cmake_minimum_required(VERSION 2.8)
project(ScorpCore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -ftemplate-backtrace-limit=0")

include_directories(include)
include_directories(${BOOST_ROOT})

add_subdirectory(source)
add_subdirectory(lib)
if(NOT WIN32)
    add_subdirectory(test)
endif()

message("SOURCE_FILES: ${SOURCE_FILES}")

add_executable(ScorpCore ${SOURCE_FILES} include/algorithm/CountReachableMarkers.h)
