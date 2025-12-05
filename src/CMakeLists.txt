cmake_minimum_required(VERSION 3.14)

add_library(functions
        fire-sim.c
        wind.c
        input.c
        console.c
        vector-math.c
        probability.c)
add_executable(main main.c)

target_link_libraries(main functions)
target_include_directories(functions PUBLIC .)
