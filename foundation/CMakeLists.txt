file (GLOB_RECURSE foundation_files_all "*.h" "*.cpp")

# See functions.cmake
add_source_group (${foundation_files_all})

add_library(foundation STATIC ${foundation_files_all})

target_link_libraries(foundation fcgi)

target_include_directories (foundation PUBLIC .)