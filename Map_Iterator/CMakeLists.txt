cmake_minimum_required(VERSION 3.15)
project(Map_Iterator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Map_Iterator main.cpp src/bst.h src/Map.h src/MapElement.h)