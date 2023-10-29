cmake_minimum_required(VERSION 3.15)

project(search_tree)

set(CMAKE_CXX_STANDARD 20)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

aux_source_directory(./src/ SRC_DIR)

add_executable(${PROJECT_NAME} ${SRC_DIR})
target_include_directories( ${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(GTest REQUIRED)
enable_testing()

find_package(Threads REQUIRED)

add_subdirectory(tests)
add_subdirectory(compare)
