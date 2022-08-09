cmake_minimum_required(VERSION 3.12)
project(breakout)

file( GLOB BREAKOUT_HEADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} includes/*)
file( GLOB BREAKOUT_SRC    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)

file( GLOB GOOGLE_TEST_HEADER RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../thirdparty/googletest/googletest/include)


add_executable( breakout main.cpp
        ${BREAKOUT_HEADER}
        ${BREAKOUT_SRC} )

target_compile_definitions(breakout PRIVATE -DNOMINMAX)
target_include_directories(breakout PRIVATE includes ${GOOGLE_TEST_HEADER})
target_link_libraries(breakout gtest gtest_main)

set_target_properties( breakout PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

add_executable( breakout_test test.cpp
        ${BREAKOUT_HEADER}
        ${BREAKOUT_SRC} )

target_compile_definitions(breakout_test PRIVATE -DNOMINMAX)
target_include_directories(breakout_test PRIVATE includes ${GOOGLE_TEST_HEADER})
target_link_libraries(breakout_test gtest gtest_main)

set_target_properties( breakout_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )