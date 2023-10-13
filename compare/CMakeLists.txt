cmake_minimum_required(VERSION 3.15)

project(compare)

aux_source_directory(./src/ SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
                            ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
)
