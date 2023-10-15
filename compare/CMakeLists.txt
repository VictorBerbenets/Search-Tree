cmake_minimum_required(VERSION 3.15)

project(compare)

add_executable(set src/set_distance.cpp)
add_executable(avl src/avl_distance.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)

set_target_properties(
    ${PROJECT_NAME} ${SET} ${AVL} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
                            ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                          )

target_include_directories(
                            avl
                            PRIVATE
                            ${INCLUDE_DIR}
                          )
