project(titsmall-test)

cmake_minimum_required(VERSION 3.2.3)

set (CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(titsmall-test main.cpp)
target_link_libraries(titsmall-test ${SDL2_LIBRARIES})
add_executable(powversi powversi.cpp)
target_link_libraries(powversi ${SDL2_LIBRARIES})
