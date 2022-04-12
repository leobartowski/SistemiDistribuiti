cmake_minimum_required(VERSION 3.21)
project(Dicembre2021 C)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(Dicembre2021 main.c)
