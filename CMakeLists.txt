cmake_minimum_required(VERSION 3.22)
project(bajr)

set(CMAKE_CXX_STANDARD 20)

add_executable(bajr src/main.cpp src/board.hpp src/board.cpp src/TTEntry.h)
