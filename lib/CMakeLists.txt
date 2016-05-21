if(NOT WIN32)
    add_subdirectory(googletest)
endif()
add_subdirectory(sqlite)

set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIRS} PARENT_SCOPE)
set(SQLITE3_SOURCE_FILES ${SQLITE3_SOURCE_FILES} PARENT_SCOPE)
