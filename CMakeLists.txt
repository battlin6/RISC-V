cmake_minimum_required(VERSION 3.15)
project(RISC_V)

set(CMAKE_CXX_STANDARD 14)

add_executable(RISC_V main.cpp text_excute.h IF.h Pipeline.h ID.h EX.h MEM.h WB.h)