cmake_minimum_required(VERSION 3.15)

project(search_tree)

set(CMAKE_CXX_STANDARD 20)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

aux_source_directory(./src/ SRC_DIR)

add_executable(search_tree_1 src/search_tree_1.cpp)
add_executable(search_tree_2 src/search_tree_2.cpp)
target_include_directories( search_tree_1 PRIVATE ${INCLUDE_DIR} )
target_include_directories( search_tree_2 PRIVATE ${INCLUDE_DIR} )

find_package(GTest REQUIRED)
enable_testing()

find_package(Threads REQUIRED)

add_subdirectory(tests)
add_subdirectory(compare)
