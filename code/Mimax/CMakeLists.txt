file(
    GLOB_RECURSE SOURCE_FILES
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "./*.c*"
    "./*.h*"
)

SourceGroupWithPaths("${SOURCE_FILES}") 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(Mimax STATIC ${SOURCE_FILES})