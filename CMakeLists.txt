cmake_minimum_required(VERSION 3.2)
project(socket-samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES common.c main.c scenario-plain.c scenario-select.c)
add_executable(socket-samples ${SOURCE_FILES} scenario.h common.h)