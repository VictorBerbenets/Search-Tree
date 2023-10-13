cmake_minimum_required(VERSION 3.15)

project("gtest")

enable_testing()
add_subdirectory(googletest)

aux_source_directory(./unit    UNIT_TESTS)
aux_source_directory(./end2end/src END2END_TESTS)

add_executable(unit ${UNIT_TESTS})
add_executable(end2end
                       ${END2END_TESTS}
                       ${TESTING_FILES}
              )

target_include_directories(end2end
                           PRIVATE end2end/include
                           PRIVATE ${INCLUDE_DIR}
                           )

target_include_directories(unit
                           PRIVATE googletest/
                           PRIVATE ${INCLUDE_DIR}
                           )
set_target_properties(
    end2end PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS "-Wall -Wextra"
)
target_link_libraries(unit gtest_main gmock_main)

