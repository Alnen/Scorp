add_subdirectory(Container)
add_subdirectory(DB)
add_subdirectory(Exceptions)
add_subdirectory(GUI)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
set(MOC ${MOC} PARENT_SCOPE)
