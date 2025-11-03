cmake_minimum_required(VERSION 3.14)
project(Forest-Fire-Prevention C)
set(CMAKE_C_STANDARD 11)

# Include dependencies

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
