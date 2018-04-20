file (GLOB_RECURSE handle_files_all "*.h" "*.cpp")

# See functions.cmake
add_source_group (${handle_files_all})

add_library(handle STATIC ${handle_files_all})

target_link_libraries(handle foundation)

target_include_directories (handle PUBLIC .)