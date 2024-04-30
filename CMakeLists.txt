cmake_minimum_required(VERSION 3.0)

project(compress24)

set(SRC_LIST compress24.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -O2 -std=c++20")

add_executable(${PROJECT_NAME} ${SRC_LIST})

