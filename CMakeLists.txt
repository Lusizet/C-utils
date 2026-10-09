cmake_minimum_required(VERSION 4.3)
project(C-utils C)

set(CMAKE_C_STANDARD 23)

add_executable(C-utils src/utils/straddon.c
        src/utils/server.c)
