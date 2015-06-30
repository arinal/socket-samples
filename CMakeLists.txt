cmake_minimum_required(VERSION 2.8)
project(socket-samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES src/common.c
    src/main.c
    src/loop-thread.c
    src/loop-select.c
    src/server.c
)

add_executable(socket-samples ${SOURCE_FILES}
    src/common.h
    src/server.h
)

target_link_libraries(socket-samples pthread)