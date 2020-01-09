file(
    GLOB_RECURSE SOURCE_FILES
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "./*.c*"
    "./*.h*"
)

SourceGroupWithPaths("${SOURCE_FILES}") 
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${MIMAX_FOLDER})
add_executable(Mimax_Test ${SOURCE_FILES})
target_link_libraries(Mimax_Test Mimax)