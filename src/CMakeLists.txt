cmake_minimum_required(VERSION 3.14)

add_library(functions
        functions.c
        wind.c
        input.c)
add_executable(main main.c)

target_link_libraries(main functions)
target_include_directories(functions PUBLIC .)
