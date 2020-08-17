cmake_minimum_required(VERSION 3.10.2)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp text_excute.h IF.h Pipeline.h ID.h EX.h MEM.h WB.h)