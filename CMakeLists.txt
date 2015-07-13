cmake_minimum_required(VERSION 2.8)
project(socket-samples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
    src/common/socket.c
    src/common/errors.c
    src/common/daemonize.c
    src/main.c
    src/loop-fork-process.c
    src/loop-thread.c
    src/loop-select.c
    src/server.c
)

add_executable(socket-samples ${SOURCE_FILES}
    src/common/common.h
    src/common/socket.h
    src/server.h
    src/common/errors.h
    src/common/daemonize.h
)

target_link_libraries(socket-samples pthread)