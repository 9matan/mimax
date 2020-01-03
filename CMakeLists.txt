cmake_minimum_required(VERSION 3.6)
project(mimax)

function(SourceGroupWithPaths SOURCE_GROUP_FILES)
    foreach(_source IN ITEMS ${SOURCE_GROUP_FILES})
        get_filename_component(_source_path "${_source}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction()

set(MIMAX_BIN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MIMAX_BIN_OUTPUT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MIMAX_BIN_OUTPUT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MIMAX_BIN_OUTPUT})

add_subdirectory(Code)