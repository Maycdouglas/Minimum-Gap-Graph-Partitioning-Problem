cmake_minimum_required(VERSION 3.20)
project(MGGPP)

set(CMAKE_CXX_STANDARD 11)

add_executable(MGGPP main.cpp Graph.h Node.h Edge.h Graph.cpp Node.cpp Edge.cpp)
