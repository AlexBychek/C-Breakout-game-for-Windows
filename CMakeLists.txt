cmake_minimum_required(VERSION 3.10)
project( breakout_project )

include_directories( ./ )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/googletest libs/googletest)
add_subdirectory(breakout_src)

