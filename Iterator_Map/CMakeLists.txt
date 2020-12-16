cmake_minimum_required(VERSION 3.15)
project(Iterator_Map)

set(CMAKE_CXX_STANDARD 20)

add_executable(Iterator_Map main.cpp bst.hpp Map.h MapElement.h)