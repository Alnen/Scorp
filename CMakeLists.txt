cmake_minimum_required(VERSION 3.3)
project(ScorpCore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -ftemplate-backtrace-limit=0")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(SOURCE_FILES source/main.cpp include/PetriNet.h include/PetriNetStorage.h include/TypeStorage.h include/TypeListStorage.h include/StateWrapper.h include/TypeList.h include/TransitionWrapper.h include/PetriNetTraits.h include/meta_utility.h include/MarkerWrapper.h include/IdObjectWrapper.h include/IntegralIdGenerator.h)
add_executable(ScorpCore ${SOURCE_FILES})