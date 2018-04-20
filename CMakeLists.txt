cmake_minimum_required (VERSION 3.10)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (functions)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")

project (hydra)

aux_source_directory(. DIR_SRCS)

add_subdirectory(foundation)
add_subdirectory(handle)

add_executable(hydra ${DIR_SRCS})

target_link_libraries(hydra handle)
