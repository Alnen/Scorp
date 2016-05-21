# TODO: put variables into cache
set(SQLITE3_INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)

set(SQLITE3_SOURCE_FILES
        ${SQLITE3_SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sqlite3.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Backup.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Column.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Database.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Statement.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Transaction.cpp
        PARENT_SCOPE)
