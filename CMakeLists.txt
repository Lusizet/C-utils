cmake_minimum_required(VERSION 4.3)
project(C-utils C)

set(CMAKE_C_STANDARD 23)

add_library(C-utils STATIC src/utils/arr_print.c
        src/utils/array_list.c
        src/utils/big_integer.c
        src/utils/ht.c
        src/utils/server.c
        src/utils/straddon.c
        src/utils/string_builder.c
)

target_link_libraries(C-utils PRIVATE ws2_32)