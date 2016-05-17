cmake_minimum_required(VERSION 2.8)
project(ScorpCore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -ftemplate-backtrace-limit=0")

include_directories(include)

add_subdirectory(source)
add_subdirectory(lib)
add_subdirectory(test)

message("SOURCE_FILES: ${SOURCE_FILES}")

add_executable(ScorpCore ${SOURCE_FILES} include/meta/TypeHolder.h include/meta/All.h include/meta/TypeWriter.h include/meta/ForEachLooper.h include/meta/RuntimeTypeSwitch.h include/container/PetriNetIterator.h include/allocator/CacheAllocator.cpp)
