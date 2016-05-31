cmake_minimum_required(VERSION 3.0.2)
project(ScorpCore)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -ftemplate-backtrace-limit=0")

include_directories(${BOOST_ROOT})

add_subdirectory(lib)
add_subdirectory(source)

include_directories(${INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIRS})

if(NOT WIN32)
    add_subdirectory(test)
endif()

message(STATUS "dfsdfs" ${MOC})
foreach(line ${MOC})
    file(READ "${line}_parameters" contents)
    string(REPLACE "\n" " " contents ${contents})
    #message(STATUS "command: " "${QT_MOC} ${contents}")
    exec_program(${QT_MOC} ARGS ${contents})
endforeach()

add_executable(ScorpCore ${SOURCE_FILES} ${SQLITE3_SOURCE_FILES} ${MOC})
add_custom_command(TARGET ScorpCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resource $<TARGET_FILE_DIR:ScorpCore>)

qt5_use_modules(ScorpCore Widgets)
