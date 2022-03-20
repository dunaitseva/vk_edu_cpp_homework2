cmake_minimum_required(VERSION 3.16)
project(HW2)

set(CMAKE_C_FLAGS "-Werror -Wpedantic -Wall -Wextra")
set(CMAKE_C_STANDARD  99)

set(TARGET transpose)
set(CMAKE_SOURCE_DIR project)

add_executable(${TARGET} ${CMAKE_SOURCE_DIR}/main.c)